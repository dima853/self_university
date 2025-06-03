package languages.java.io.part1.chapter2.mini_projects;

import java.io.*;

public class FileCopyWithProgress {

    public static void copyFile(String source, String destination) throws IOException {
        File sourceFile = new File(source);
        long fileSize = sourceFile.length();
        try (InputStream in = new BufferedInputStream(new FileInputStream(source));
             OutputStream out = new BufferedOutputStream(new FileOutputStream(destination))) {

            byte[] buffer = new byte[8192]; // 8KB buffer
            long totalBytesRead = 0;
            int bytesRead;

            while ((bytesRead = in.read(buffer)) != -1) {
                out.write(buffer, 0, bytesRead);
                totalBytesRead += bytesRead;

                // Calculate and display progress
                double progress = (double) totalBytesRead / fileSize * 100;
                System.out.printf("Progress: %.2f%%\r", progress);
            }
            System.out.println(); // Move to the next line after completion
        }
    }

    public static void main(String[] args) {
        String sourceFile = "source.txt";
        String destinationFile = "destination.txt";

        // Create a dummy source file
        try (PrintWriter writer = new PrintWriter(new FileWriter(sourceFile))) {
            for (int i = 0; i < 100000; i++) {
                writer.println("This is a line of text.");
            }
        } catch (IOException e) {
            System.err.println("Error creating dummy file: " + e.getMessage());
        }

        try {
            copyFile(sourceFile, destinationFile);
            System.out.println("File copied successfully!");
        } catch (IOException e) {
            System.err.println("Error during file copy: " + e.getMessage());
        }
    }
}