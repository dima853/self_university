# 1.1.2 The Stream Classes 

It is important to note that these classes **do not belong** to the Stream API introduced in Java 8 ('java.util.stream'). They are part of an older byte-level I/O stream system.

| Class                     | Type   | Description                                                                                                                                                                                 |
| ------------------------- | ------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `BufferedInputStream`     | Input  | Adds buffering to another InputStream, improving performance by reducing the number of reads from the base stream.                                                                          |
| `BufferedOutputStream`    | Output | Adds buffering to another OutputStream, improving performance by reducing the number of writes to the base stream.                                                                          |
| `ByteArrayInputStream`    | Input  | Allows you to read bytes from the byte[] array.                                                                                                                                             |
| `ByteArrayOutputStream`   | Output | Allows you to write bytes to a dynamically expanding byte[] array.                                                                                                                          |
| `DataInputStream`         | Input  | Allows you to read primitive Java data types (int, long, float, double, boolean) from a stream.                                                                                             |
| `DataOutputStream`        | Output | Allows you to write primitive Java data types (int, long, float, double, boolean) to a stream in a machine-independent format.                                                              |
| `FileInputStream`         | Input  | Allows you to read bytes from a file.                                                                                                                                                       |
| `FileOutputStream`        | Output | Allows you to write bytes to a file.                                                                                                                                                        |
| `FilterInputStream`       | Input  | An abstract class that is the base for all filtering InputStreams (for example, BufferedInputStream, DataInputStream). Allows you to add additional functionality to the basic InputStream. |
| `FilterOutputStream`      | Output | An abstract class that is the base for all filtering OutputStreams. Allows you to add additional functionality to the basic OutputStream.                                                   |
| `LineNumberInputStream`   | Input  | (Deprecated class) Tracks line numbers when reading from a stream. It is recommended to use `BufferedReader` and keep track of the line numbers yourself.                                   |
| `ObjectInputStream`       | Input  | Allows you to read Java objects from a stream using deserialization.                                                                                                                        |
| `ObjectOutputStream`      | Output | Allows you to write Java objects to a stream using serialization.                                                                                                                           |
| `PipedInputStream`        | Input  | is used together with `PipedOutputStream` to create a communication channel between two threads within the same JVM (inter-thread communication).                                           |
| `PipedOutputStream`       | Output | is used together with `PipedInputStream` to create a communication channel between two threads within the same JVM (inter-thread communication).                                            |
| `PrintStream`             | Output | Allows you to write formatted text to a stream. The classes `System.out` and `System.err` are instances of `PrintStream`.                                                                   |
| `PushbackInputStream`     | Input  | Allows bytes to be "returned" back to the stream so that they can be read again.                                                                                                            |
| `SequenceInputStream`     | Input  | Allows you to combine multiple InputStreams into one by reading them sequentially.                                                                                                          |
| `StringBufferInputStream` | Input  | (Deprecated class) Allows you to read characters from the `StringBuffer'. It is recommended to use `StringReader` instead.                                                                  |

**Key points:**

* **`InputStream` and `OutputStream`:** These are basic abstract classes for reading and writing bytes.
* **Filtering:** Many classes (for example, `BufferedInputStream`, `DataInputStream`) are filtering streams that add functionality to other streams.
* **Bytes:** These classes operate at the byte level. To work with characters, use the `Reader` and `Writer' classes.
* **Obsolete classes:** Some classes (for example, `LineNumberInputStream`, `StringBufferInputStream`) are deprecated and are not recommended for use.