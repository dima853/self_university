package languages.java.io.part1.chapter2.mini_projects;

import java.io.*;

public class CaesarCipher {

    public static void encrypt(String inputFile, String outputFile, int key) throws IOException {
        try (InputStream in = new FileInputStream(inputFile);
             OutputStream out = new FileOutputStream(outputFile)) {
            int byteData;
            while ((byteData = in.read()) != -1) {
                byte encryptedByte = (byte) ((byteData + key) % 256);
                out.write(encryptedByte);
            }
        }
    }

    public static void decrypt(String inputFile, String outputFile, int key) throws IOException {
        encrypt(inputFile, outputFile, 256 - key); // Decryption is just encryption with the inverse key
    }

    public static void main(String[] args) {
        String inputFile = "input.txt";
        String encryptedFile = "encrypted.txt";
        String decryptedFile = "decrypted.txt";
        int key = 3;

        try {
            // Create an example input file
            try (PrintWriter writer = new PrintWriter(new FileWriter(inputFile))) {
                writer.print("Hello, Caesar Cipher!");
            }

            // Encrypt the file
            encrypt(inputFile, encryptedFile, key);
            System.out.println("File encrypted successfully to " + encryptedFile);

            // Decrypt the file
            decrypt(encryptedFile, decryptedFile, key);
            System.out.println("File decrypted successfully to " + decryptedFile);

        } catch (IOException e) {
            System.err.println("Error during file operations: " + e.getMessage());
        }
    }
}