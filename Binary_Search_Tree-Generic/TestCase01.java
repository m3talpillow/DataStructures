// Sean Szumlanski
// COP 3503, Fall 2017

// ===========================
// GenericBST: TestCase01.java
// ===========================
// A brief test case to help ensure you've implemented the difficultyRating()
// and hoursSpent() methods correctly, and that you can create and use a
// GenericBST.
//
// For detailed compilation and testing instructions, see the assignment PDF.


import java.io.*;
import java.util.*;

public class TestCase01
{
	public static void main(String [] args)
	{
		double difficulty = GenericBST.difficultyRating();
		System.out.println((difficulty < 1.0 || difficulty > 5.0) ? "fail whale :(" : "Hooray!");

		double hours = GenericBST.hoursSpent();
		System.out.println((hours <= 0.0) ? "fail whale :(" : "Hooray!");

		// Create a GenericBST.
		GenericBST<Integer> myTree = new GenericBST<Integer>();

		int [] array = {90, 54, 74, 52, 99, 6, 3, 43, 55, 67};

		for (int i = 0; i < array.length; i++)
			myTree.insert(array[i]);

		myTree.inorder();
		myTree.preorder();
		myTree.postorder();
	}
}
