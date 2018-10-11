// Jared Johansson
// COP 3503, Fall 2017

// You know how to node?
// Simple node verifications

import java.io.*;
import java.util.*;

public class TestCase19
{
  public static void main(String [] args)
  {
    Node<Integer> node1 = new Node<>(2); // h: 2
    Node<Integer> node2 = new Node<>(35, 3); // d: 35, h: 3

    // Creation check
    System.out.println("node1 size: "+node1.height());
    System.out.println("node1 value: "+node1.value());

    System.out.println("node2 size: "+node2.height());
    System.out.println("node2 value: "+node2.value());

    // Get and Set nexts
    int h = 0;
    System.out.println("node1 nexts: ");
    while (h < node1.height())
    {
      // Create a new node, cap 1
      node1.setNext(h, new Node<Integer>(0));
      // Recieve the address that was just created and placed
      System.out.println("next("+h+"): "+node1.next(h));
      h++;
    }

    System.out.println("node2 nexts: ");
    h = 0;
    while (h < node2.height())
    {
      // Create a new node, cap 1
      node2.setNext(h, new Node<Integer>(0));
      // Recieve the address that was just created and placed
      System.out.println("next("+h+"): "+node2.next(h));
      h++;
    }

    // Grow and remove
    System.out.println("node1 array size: "+node1.height());
    node1.grow();
    node1.setNext(2, new Node<Integer>(3));
    System.out.println("node1 array size: "+node1.height());
    node1.trim(1);
    System.out.println("node1 array size: "+node1.height());
  }
}
