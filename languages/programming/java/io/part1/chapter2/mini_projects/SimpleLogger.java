package languages.java.io.part1.chapter2.mini_projects;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;

/*
**1. A simple logger to a file:**

This project is the simplest, and its code is relatively short. It demonstrates the basic use of an `OutputStream` to write data to a file.

```java
public class SimpleLogger {
    private final String logFilePath;
```

* `public class SimpleLogger { ... }`: Declaration of the class `SimpleLogger'.
*   `private final String logFilePath;`:
    * Declaration of a private (access only inside the class) and `final' (the value is set only once in the constructor) field `logFilePath` of type `String'. This will be the path to the log file.

```java
    public SimpleLogger(String logFilePath) {
        this.logFilePath = logFilePath;
    }
```

* `public SimpleLogger(String logFilePath) { ... }`: Constructor of the class.  It takes the path to the log file as an argument.
* `this.logFilePath = logFilePath;`: Assigns the value of the `logFilePath` argument to the `logFilePath` field of the current object.

```java
    public void log(String message) {
        try (OutputStreamWriter writer = new OutputStreamWriter(new FileOutputStream(logFilePath, true))) {
            writer.write(message + "\n");
        } catch (IOException e) {
            System.err.println("Error writing to the log file: " + e.GetMessage());
        }
    }
```

* `public void log(String message) { ... }`: The `log` method that writes the message to the log file.
*   `try ( ... ) { ... } catch (IOException e) { ... }`: The `try-catch` block for handling possible exceptions (errors).
    * `try (OutputStreamWriter writer = new OutputStreamWriter(new FileOutputStream(logFilePath, true))) { ... }`: The 'try-with-resources` block. It automatically closes the `writer' resource after executing the 'try` block, even if an exception occurs.
        *   `OutputStreamWriter writer = new OutputStreamWriter(new FileOutputStream(logFilePath, true))`: Creating an instance of `OutputStreamWriter'.
            * `new FileOutputStream(logFilePath, true)`: Creating A `FileOutputStream'.
                * `logFilePath': The path to the log file.
                * `true`: The `true` argument means that the data will be appended to the file (append), rather than overwriting it.
            *   `new OutputStreamWriter(...)`: Creates an `OutputStreamWriter' that converts Java (Unicode) characters into bytes before writing them to a `FileOutputStream`.
        * `writer.write(message + "\n");`: Writes the message to the log file by adding a newline character (`\n`).
    * `catch (IOException e) { ... }`: Handling the `IOException` exception that may occur when working with a file.
        * `System.err.println("Error writing to the log file: " + e.GetMessage());`: Outputs an error message to the standard error stream (stderr). `e.GetMessage()` provides information about the error.

```java
    public static void main(String[] args) {
        SimpleLogger logger = new SimpleLogger("app.log");
        logger.log("The application is running");
        logger.log("An important operation has been performed");
        logger.log("Application completed");
    }
}
```

* `public static void main(String[] args) { ... }`: The main method of the program.
* `SimpleLogger logger = new SimpleLogger("app.log");`: Create an instance of `SimpleLogger` with the file name "app.log".
* `logger.log("Application running");`: Calling the `log` method to write the message to a file.
*   `logger.log("An important operation has been performed");`: Calling the `log` method again.
* `logger.log("Application terminated");`: Calling the `log` method again.

**How it works:**

1. The program creates a `SimpleLogger' object, specifying the file name for the log.
2. The `log` method takes a line (message) and writes it to a file, adding a new line after each message.
3. 'FileOutputStream` is used to open the file in append mode (`true` in the constructor).
4. `OutputStreamWriter` is used to convert characters to bytes and write them to the file.
5. The 'try-with-resources` block ensures that 'OutputStreamWriter' is automatically closed, freeing up resources.
6. In the `main()` method, an instance of `SimpleLogger` is created, and then several messages are written to the log file.
* */
public class SimpleLogger {
    private final String logFilePath;

    public SimpleLogger(String logFilePath) {
        this.logFilePath = logFilePath;
    }

    public void log(String message) {
        try (OutputStreamWriter writer = new OutputStreamWriter(new FileOutputStream(logFilePath, true))) {
            writer.write(message + "\n");
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public static void main(String[] args) {
        SimpleLogger logger = new SimpleLogger("app.log");
        logger.log("Приложение запущено");
        logger.log("Выполнена важная операция");
        logger.log("Приложение завершено");
    }
}