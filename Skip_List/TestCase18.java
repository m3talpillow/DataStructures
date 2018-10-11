// Jared Johansson
// COP 3503, Fall 2017

// Can you insert?

import java.io.*;
import java.util.*;

public class TestCase18
{
  public static void main(String [] args)
  {
    SkipList<Integer> skip = new SkipList<>();

    skip.insert(3);
    skip.printList();
    System.out.println();

    skip.insert(4);
    skip.printList();
    System.out.println();

    skip.insert(5);
    skip.printList();
    System.out.println();

    skip.insert(7);
    skip.printList();
    System.out.println();

    skip.insert(6);
    skip.printList();
    System.out.println();
  }
}
