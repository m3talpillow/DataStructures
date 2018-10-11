// Jared Johansson
// COP 3503, Fall 20

import java.io.*;
import java.util.*;

public class TestCase04
{
	public static void main(String [] args)
	{
		// Create a GenericBST.
    GenericBST<myType> myTree = new GenericBST<myType>();

		myType [] array = {3.14, 1.67, 6.66, 6.9, 3.0, 34.8};

		for (int i = 0; i < array.length; i++)
			myTree.insert(array[i]);

		myTree.inorder();
		myTree.preorder();
		myTree.postorder();
	}
}

public class myType implements comparable<myType>
{

}
