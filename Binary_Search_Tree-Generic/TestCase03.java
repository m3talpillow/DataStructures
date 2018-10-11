// Jared Johansson
// COP 3503, Fall 20

import java.io.*;
import java.util.*;

public class TestCase03
{
	public static void main(String [] args)
	{
		// Create a GenericBST.
    GenericBST<Double> myTree = new GenericBST<Double>();

		double [] array = {3.14, 1.67, 6.66, 6.9, 3.0, 34.8};

		for (int i = 0; i < array.length; i++)
			myTree.insert(array[i]);

		myTree.inorder();
		myTree.preorder();
		myTree.postorder();
	}
}
