// Jared Johansson
// Fall 2017, COP3503
// getMaxHeight checks

import java.io.*;
import java.util.*;

public class TestCase20
{
	public static void main(String [] args)
	{
		for (int i = 1; i < 34; i++)
			System.out.println("i: "+i+" -> "+SkipList.getMaxHeight(i));
  }
}
