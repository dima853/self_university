# The OutputStream Class

## Summary of the `java.io.OutputStream` class

'java.io.OutputStream` is an abstract class that is the base class for all classes designed to write byte data to a stream. It defines the basic methods for writing data, as well as for closing and resetting the stream.

**Main Features:**

* **Abstract:** You cannot create an instance of `OutputStream` directly. You need to use its subclasses, which provide a specific implementation of the `write(int b)` method.
* **Byte stream:** Designed to work with bytes (8-bit values).
*   **Data output:** Allows you to write data to various destinations such as files, network connections, memory, etc.

**Basic methods (with annotation and examples):**

1.  **`public abstract void write(int b) throws IOException`**

    * **Description:** An abstract method that **must be implemented** in subclasses. Writes one byte of data to the stream. The argument `b` is an integer, but only the lowest byte (8 bits) is written.
    * **Parameters:**
        * `b': An integer representing a byte to be written (range 0-255).
* **Exceptions:**
        * 'IOException': Thrown in case of an input/output error (for example, when writing to a file that does not exist or cannot be accessed).
    * **Example:**

    ```java
    import java.io.FileOutputStream;
    import java.io.IOException;
    import java.io.OutputStream;

    public class WriteByteToFile {
        public static void main(String[] args) {
            try (OutputStream outputStream = new FileOutputStream("data.txt")) { // try-with-resources
                OutputStream.write(65); // Writing a byte representing the character 'A' (ASCII 65)
                OutputStream.write(66); // Writing a byte representing the character 'B' (ASCII 66)
OutputStream.write(67); // Writing a byte representing the character 'C' (ASCII 67)
} catch (IOException e) {
                System.err.println("Error writing to file: " + e.GetMessage());
            }
        }
    }
    ```

    * **Note:** The 'try-with-resources' construct is used, which automatically closes the thread after executing the `try' block. This is recommended to prevent resource leaks.

2.  **`public void write(byte[] data) throws IOException`**

    * **Description:** Writes an array of bytes to a stream.
    * **Parameters:**
        * `data': An array of bytes to be written.
    * **Exceptions:**
        * 'IOException': Is thrown in case of an input/output error.
    * **Example:**

    ```java
    import java.io.FileOutputStream;
    import java.io.IOException;
    import java.io.OutputStream;

    public class WriteByteArrayToFile {
        public static void main(String[] args) {
            byte[] message = {72, 101, 108, 108, 111}; // Hello
            try (OutputStream outputStream = new FileOutputStream("message.txt")) {
                outputStream.write(message);
            } catch (IOException e) {
                System.err.println("Error writing to file: " + e.GetMessage());
            }
        }
    }
    ```

3.  **`public void write(byte[] data, int offset, int length) throws IOException`**

    * **Description:** Writes part of the byte array to the stream.
    * **Parameters:**
        * `data': An array of bytes.
        * `offset': The initial index in the 'data` array from which the record starts.
        * `length': The number of bytes to write from the 'data` array.
    * **Exceptions:**
        * 'IOException': Is thrown in case of an input/output error.
    * **Example:**

    ```java
    import java.io.FileOutputStream;
    import java.io.IOException;
    import java.io.OutputStream;

    public class WritePartOfByteArrayToFile {
        public static void main(String[] args) {
            byte[] message = {72, 101, 108, 108, 111, 32, 87, 111, 114, 108, 100}; // Hello World
            try (OutputStream outputStream = new FileOutputStream("part_message.txt")) {
                OutputStream.write(message, 0, 5); // Writing only "Hello"
} catch (IOException e) {
                System.err.println("Error writing to file: " + e.GetMessage());
            }
        }
    }
    ```

4.  **`public void flush() throws IOException`**

    * **Description:** Forcibly flushes buffered data to the stream. Some threads use a buffer to improve performance. The `flush()` method forces all data from the buffer to be written to the target destination.
    * **Exceptions:**
        * 'IOException': Is thrown in case of an input/output error.
    * **Example:**

    ```java
    import java.io.BufferedOutputStream;
    import java.io.FileOutputStream;
    import java.io.IOException;
    import java.io.OutputStream;

    public class FlushExample {
        public static void main(String[] args) {
            try (OutputStream outputStream = new BufferedOutputStream(new FileOutputStream("buffered_data.txt"))) {
                String message = "This is some buffered data.";
                byte[] data = message.getBytes();
                outputStream.write(data);

                // Forcibly flushing the buffer to a file
                outputStream.flush();

                System.out.println("Data is written to the buffer and flushed to a file.");
            } catch (IOException e) {
                System.err.println("Error writing or resetting data: " + e.GetMessage());
            }
        }
    }
    ```
5.  **`public void close() throws IOException`**

    * **Description:** Closes the stream and releases all associated resources (e.g. file descriptor, network connection). It is important to always close threads after use to prevent resource leaks.
    * **Exceptions:**
        * 'IOException': Thrown in case of an I/O error when closing the stream.
    * **Example:**
    ```java
    import java.io.FileOutputStream;
    import java.io.IOException;
    import java.io.OutputStream;

    public class CloseExample {
        public static void main(String[] args) {
            OutputStream outputStream = null;
            try {
                outputStream = new FileOutputStream("data.txt");
                String message = "Some data to write.";
                outputStream.write(message.getBytes());
            } catch (IOException e) {
                System.err.println("Error writing to file: " + e.GetMessage());
            } finally {
// Closing the stream in the finally block so that it is always closed
                if (outputStream != null) {
                    try {
                        outputStream.close();
                        System.out.println("Stream is closed.");
                    } catch (IOException e) {
                        System.err.println("Error closing the stream: " + e.GetMessage());
                    }
                }
            }
        }
    }
    ```

    * **Note:** Using the `try-finally` block ensures that the stream is closed, even if an exception occurs in the 'try` block. However, as mentioned earlier, the `try-with-resources` construction is the preferred way to work with resources that need to be closed.

**Subclasses of `OutputStream`:**

'OutputStream' has many subclasses that implement specific ways of writing data.  Here are some of the most commonly used:

* `FileOutputStream': For writing data to files.
* 'ByteArrayOutputStream': To write data to an array of bytes in memory.
* `ObjectOutputStream': For writing objects to a stream (serialization).
* `PipedOutputStream': For writing data to a channel (used in multithreaded programming).
* `BufferedOutputStream': For buffered data recording, improving performance.
* `DataOutputStream': For writing primitive data types (int, double, boolean) to a stream in binary format.

**Example of polymorphism and implementation hiding:**

```java
import java.io.IOException;
import java.io.OutputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;

public class URLConnectionExample {
    public static void main(String[] args) {
        try {
            URL url = new URL("https://www.example.com");
            URLConnection connection = url.openConnection();
            OutputStream outputStream = connection.getOutputStream();

            // We don't know (and shouldn't know) the exact type of OutputStream
            // It only matters that it is an OutputStream
            String data = "some data to send";
            outputStream.write(data.getBytes());
            outputStream.close();

            System.out.println("Data has been sent to the server.");

        } catch (MalformedURLException e) {
            System.err.println("Invalid URL: " + e.GetMessage());
        } catch (IOException e) {
            System.err.println("Connection or write error: " + e.getMessage());
        }
    }
}
```

* In this example, we get the `OutputStream` from the `URLConnection'.  We don't know which specific subclass `OutputStream' is returned.  The only important thing is that it is an object of type `OutputStream`, and we can use the methods defined in `OutputStream` (for example, `write()` and `close()`) to send data to the server.  A specific implementation of `OutputStream` (for example, `sun.net.www.http.KeepAliveStream`) is hidden from us.

**Inheritance and documentation:**

It is important to remember that classes inherit methods from their superclasses.  For example, 'java.io.DataOutputStream' does not declare the `close()` method, but it inherits it from `OutputStream`.  Therefore, even if the method is not specified in the documentation for a specific class, it can be accessed through inheritance.  Always refer to the class hierarchy to see all available methods.

**In conclusion:**

'java.io.OutputStream' is a fundamental class for writing byte data in Java. Understanding its basic methods and principles of operation is necessary to develop applications that work with input/output streams.  Don't forget about inheritance and always close the streams after use!