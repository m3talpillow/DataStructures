// Jared Johansson
// Fall 2017 - COP3503
// Test case for getMaxHeight

import java.io.*;
import java.util.*;

public class TestCase17
{
	public static void main(String [] args)
	{
    System.out.println(SkipList.getMaxHeight(16));
    System.out.println(SkipList.getMaxHeight(4));
    System.out.println(SkipList.getMaxHeight(7));
    System.out.println(SkipList.getMaxHeight(8));
    System.out.println(SkipList.getMaxHeight(15));
  }
}
