// Jared Johansson
// Fall 2017 - COP3503

import java.util.ArrayList;
import java.util.Random;

// Node class for use inside SkipList
class Node<T extends Comparable<T>>
{
  private static final boolean DB = false;
  private T data;
  private int height;
  private ArrayList<Node<T>> list;

  // Base constructor, forces size 1 or greater
  Node(int height)
  {
    this.height = (height < 1) ? 1 : height;
    this.list = new ArrayList<>(1);

    // Create indexes because ArrayList is dumb
    for (int i = 0; i < this.height; i++)
      this.list.add(null);

    if (DB) System.out.println("SYS: Node created, height: "+this.height);
  }

  // Places data, deffers to base constructor
  Node(T data, int height)
  {
    this(height);
    this.data = data;
    if (DB) System.out.println("SYS: Node created, data: "+this.data);
  }

  // Getter for data
  public T value()
  {
    return this.data;
  }

  // Getter for height
  public int height()
  {
    return this.height;
  }

  // Grabs next node link at specified level
  public Node<T> next(int level)
  {
    // Level is the index, 0 - (height - 1)
    if (level < 0 || level > this.height - 1)
      return null;

    return this.list.get(level);
  }

  // Sets next node link at specified level
  public void setNext(int level, Node<T> node)
  {
    if (level < 0 || level > this.height - 1 || node == null)
      return;

    if (DB) System.out.println("SYS: setNext(level: "+level+")");
    // Overwrites data
    this.list.set(level, node);
  }

  // Increases height by 1
  public void grow()
  {
    this.height++;
    this.list.add(null);
    if (DB) System.out.println("SYS: List grew, size: "+this.height);
  }

  // Increases height by 1 if pass 50/50 test
  public void maybeGrow()
  {
    Random ran = new Random();

    // Non-static method
    if (ran.nextBoolean())
      grow();
  }

  // Decreases height to specified number
  public void trim(int height)
  {
    if (height < 1 || height >= this.height)
      return;

    // Keep removing until heights equal
    while (this.height > height)
    {
      this.list.remove(this.height - 1);
      this.height--;
    }

    // Fix array capacity
    this.list.trimToSize();
  }
}



// DUN, DUN, DHUNNNN. SkipList.
public class SkipList<T extends Comparable<T>>
{
  private static final boolean DB = false;
  private Node<T> head;
  private int size;

  // Base constructor, creates empty head with height 1
  SkipList()
  {
    this.head = new Node<T>(1);
    this.size = 0;
    if (DB) System.out.println("SYS: Head created.");
  }

  // Constructor to specify height of head node
  SkipList(int height)
  {
    this.head = new Node<T>(height);
    this.size = 0;
    if (DB) System.out.println("SYS: Head created.");
  }

  // Getter for how many nodes in list
  public int size()
  {
    return this.size;
  }

  // Getter for max height of skiplist
  public int height()
  {
    return this.head.height();
  }

  // Getter for the head of the list
  public Node<T> head()
  {
    return this.head;
  }

  // Insert data with any appropriate height
  public void insert(T data)
  {
    // Create new node with data and random height
    Node<T> node = new Node<T>(data, SkipList.generateRandomHeight(this.height()));
    insertHelper(node);
  }

  // Insert data with a specific height
  public void insert(T data, int height)
  {
    // Create new node with specific height
    Node<T> node = new Node<T>(data, height);
    insertHelper(node);
  }

  // Heavy lifting of insert functions
  private void insertHelper(Node<T> node)
  {
    // Check if new level needed, then grow list if so
    if (getMaxHeight(this.size + 1) > this.height())
    {
      // If new node is maxed height, give grow chance
      if (this.height() == node.height())
        node.maybeGrow();
      growSkipList();
    }

    // Traverse list, find insert location
    Node<T> aux = this.head;
    int h = 1;
    while (h <= this.height())
    {
      int depth = this.height() - h;
      // Next value is less than new value, move forward
      if (aux.next(depth) != null && aux.next(depth).value().compareTo(node.value()) < 0)
        aux = aux.next(depth);

      // If next value is larger, or same, drop a level
      else
      {
        // Save re-links if at correct height
        if (node.height() - 1 >= depth)
        {
          // Copy aux's next address, new node separates
          node.setNext(depth, aux.next(depth));
          // Set aux's next to new node
          aux.setNext(depth, node);
        }
        h++;
      }
    }

    this.size++;
    if (DB) System.out.println("SYS: insert, size: "+this.size);
  }

  // Given data, find and remove node from list
  public void delete(T data)
  {
    int h = 1;
    boolean flag = false;
    Node<T> aux = this.head;
    ArrayList<Node<T>> saves = new ArrayList<>(this.height());

    // Traverse list
    while (h <= this.height())
    {
      int depth = this.height() - h;
      // Look forward, if next value is less than data, advance
      if (aux.next(depth) != null && aux.next(depth).value().compareTo(data) < 0)
        aux = aux.next(this.height() - h);

      // If next value is greater or non-existant, drop level
      else if (aux.next(depth) == null || aux.next(depth).value().compareTo(data) > 0)
        h++;

      // If target is found save nodes where links are and drop level
      else
      {
        // Save nodes that will need to be re-linked
        saves.add(0, aux);
        h++;
        flag = true;
      }
    }

    // Target node is now unreachable and eligible for GC
    if (flag)
    {
      // Move to node to be deleted
      aux = aux.next(0);
      // Re-link all saved nodes around target node
      for (int i = 0; i < aux.height(); i++)
        saves.get(i).setNext(i, aux.next(i));

      this.size--;
      // Check if trim needed
      if (getMaxHeight(this.size) < this.height())
        trimSkipList();
    }

    if (DB) System.out.println("SYS: delete, size: "+this.size);
  }

  // Find if list holds specific data
  public boolean contains(T data)
  {
    if (DB) System.out.println("SYS: contains -> get");
    if (get(data) == null)
      return false;

    return true;
  }

  // Find location of specific data
  public Node<T> get(T data)
  {
    Node<T> aux = this.head;
    int h = 1;

    while (aux != null && h <= this.height())
    {
      // Look forward, if less than data, move forward
      if (aux.next(this.height() - h) != null && aux.next(this.height() - h).value().compareTo(data) < 0)
        aux = aux.next(this.height() - h);

      // If aux is greater than data or next doesn't exist, drop a level
      else if (aux.next(this.height() - h) == null || aux.next(this.height() - h).value().compareTo(data) > 0)
        h++;

      // If equal return node
      else
      {
        if (DB) System.out.println("SYS: get found: "+data);
        return aux.next(this.height() - h);
      }
    }

    if (DB) System.out.println("SYS: get couldn't find: "+data);
    return null;
  }

  // Given n nodes, what is the height
  public static int getMaxHeight(int n)
  {
    if (n <= 2)
      return 1;

    // Adds up current level's number of nodes, until passes n
    int level = 1, val = 2, mult = 2;
    while (val < n)
    {
      val = val + mult;
      mult *= 2;
      level++;
    }

    if (DB) System.out.println("SYS: Found max height, "+n+" nodes: "+level);
    return level;
  }

  // Gets a height from 1 to maxHeight
  private static int generateRandomHeight(int maxHeight)
  {
    int height = 1;
    Random ran = new Random();

    // Rolls 50/50 and if pass, continues
    while (height < maxHeight)
    {
      if (ran.nextBoolean())
        height++;
      else
        break;
    }

    if (DB) System.out.println("SYS: Generated height: "+height);
    return height;
  }

  // Gives chances for maxed nodes to grow
  private void growSkipList()
  {
    // Grow head node
    this.head.grow();

    // Create aux node to travel list
    Node<T> aux;
    if (this.head.next(0) != null)
      aux = this.head.next(0);
    else
      return;
    // Keep track of previously accended node
    Node<T> prev = this.head;

    // Traverse list
    while (aux != null)
    {
      // If node height is list height (old), its maxed
      if (aux.height() == this.height() - 1)
      {
        aux.maybeGrow();
        // If node grew, re-link
        if (aux.height() == this.height())
        {
          aux.setNext(this.height() - 1, prev.next(this.height() - 1));
          prev.setNext(this.height() - 1, aux);
          prev = aux;
        }
      }
      // Move aux to next node
      aux = aux.next(0);
    }

    if (DB) System.out.println("SYS: Skiplist grew, height: "+this.height());
  }

  // Trims all maxed nodes in list down one height FIX, O(n) -> lg n
  private void trimSkipList()
  {
    int height = SkipList.getMaxHeight(this.size);
    // Create node to travel list
    Node<T> aux = this.head;

    // Traverse list, trimming when needed
    while (aux != null)
    {
      aux.trim(height);
      aux = aux.next(0);
    }

    if (DB) System.out.println("SYS: Skiplist trimed, height: "+this.height());
  }

  // Prints value and height of each node
  public void printList()
  {
    int i = 0;
    Node<T> aux = this.head;

    while(aux != null)
    {
      System.out.println("Node "+i+": "+aux.value()+", H: "+aux.height());
      i++;
      aux = aux.next(0);
    }
  }

  public static double difficultyRating()
  {
    return 4.0;
  }

  public static double hoursSpent()
  {
    return 10.0;
  }
}
