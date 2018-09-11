package findinglongestPath;

public class longestPath {
  import java.io.BufferedReader;
  import java.io.InputStreamReader;
  import java.util.ArrayList;
  import java.io.IOException;
 
  class NonDecreasing {

    /**
     * Take a rectangular grid of numbers and find the length
     * of the longest sub-sequence.
     * @return the length as an integer.
     */
    static int [][]maze; 
    public static int longestSequence(int[][] grid) {
      int longestSequence = 0; 
      maze = grid; 
      
      return 0;
    }

    public static void main(String[] args) throws IOException {
      BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));

      int numRows = 0;
      int numCols = 0;
      String[] firstLine = reader.readLine().split("\\s+");
      numRows = Integer.parseInt(firstLine[0]);
      numCols = Integer.parseInt(firstLine[1]);

      int[][] grid = new int[numRows][numCols];

      for (int row = 0; row < numRows; row++) {
          String[] inputRow = reader.readLine().split("\\s+");

          for (int col = 0; col < numCols; col++) {
              grid[row][col] = Integer.parseInt(inputRow[col]);
          }
      }
      int length = longestSequence(grid);
      System.out.println(length);
    }
  }

