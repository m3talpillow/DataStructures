// Jared Johansson
// COP 3503, Fall 20

import java.io.*;
import java.util.*;

public class TestCase02
{
	public static void main(String [] args)
	{
		// Create a GenericBST.
		GenericBST<String> myTree = new GenericBST<String>();

		String[] array = {"hello", "world", "i", "am", "a", "computer", "bow", "before", "my", "greatness"};

		for (int i = 0; i < array.length; i++)
			myTree.insert(array[i]);

		myTree.inorder();
		myTree.preorder();
		myTree.postorder();
	}
}
