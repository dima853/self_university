# Item 1: Consider static factory methods instead of constructors (Effective Java)

This is the first and one of the most important tips in Joshua Bloch's book Effective Java. Let's analyze it in detail.

Note that a static factory method is not the same as the Factory Method pattern from Design Patterns [Gamma95]. 
The static factory method described in this item has no direct equivalent in Design Patterns.

## What is the static factory method?

A static factory is just a static method that returns an instance of a class. This is not the same as the "Factory" pattern from GoF.

### How not to (bad example):

```java
public class User {
    private String name;
    
    public User(String name) {
        this.name = name;
    }
    
    // Usage:
// User user = new User("John");
}
```

### How it should be (good example):

```java
public class User {
    private String name;
    
    // Private Constructor
    private User(String name) {
        this.name = name;
    }
    
    // Static Factory
    public static User createWithName(String name) {
        return new User(name);
    }
    
    // Usage:
// User user = User.createWithName("John");
}
```

## Advantages of static factory methods

1. **Have meaningful names**
- Constructors should always be named the same as a class, and factory methods can have descriptive names.
   - Example: `BigInteger.probablePrime()` is better than `new BigInteger(...)`

2. **You are not required to create a new object every time**
   - You can cache objects and return them on repeated calls.
   - Example: `Boolean.valueOf()` does not create a new object
   - It can greatly improve performance if equivalent objects are requested often, especially if they are expensive to create.

3. **Can return an object of any subtype**
   - The constructor can only return an object of its own class
   - Example: `Collections.unmodifiableList()` returns the implementation of List, but not the one you passed.

4. **They can reduce the verbosity of creating parameterized instances.**
   - Example: `Map<String, List<String>> m = new HashMap<String, List<String>>();` vs
     `Map<String, List<String>> m = HashMap.newInstance();`

## Disadvantages of static factory methods

1. **A class without public/protected constructors cannot be inherited**
   - It can also be an advantage (encouraging composition)

2. **Does not differ from other static methods**
   - They are not highlighted as constructors in the documentation
- Solution: use common names:
     - `valueOf' - conversion (usually returns the same type)
- `of` - compact alternative to valueOf (EnumSet)
     - `getInstance' - returns an instance, possibly with parameters
     - `newInstance' - guarantees a new instance
     - `GetType`/`newType' - when the method is in another class

## Advanced examples

### Example with caching:

```java
public class Color {
    private final int rgb;
    private static final Map<Integer, Color> CACHE = new HashMap<>();
    
    private Color(int rgb) {
        this.rgb = rgb;
    }
    
    public static Color fromRgb(int rgb) {
        return CACHE.computeIfAbsent(rgb, k -> new Color(rgb));
    }
}
```

### Example of returning subtypes:

```java
public abstract class Shape {
    public static Shape createCircle(double radius) {
        return new Circle(radius);
    }
    
    public static Shape createRectangle(double width, double height) {
        return new Rectangle(width, height);
    }
    
    private static class Circle extends Shape { /* ... */ }
    private static class Rectangle extends Shape { /* ... */ }
}
```

## When to use it?

1. When creating an object requires complex logic
2. When it is important to control the number of instances created
3. When you need to provide convenient ways to create an object
4. When you need to hide the implementation behind the interface

## Output

Static factory methods are a powerful alternative to constructors. They make the API more flexible and user-friendly. 
Although they do not completely replace constructors, they are preferable in most cases.


# Additional materials
- Flyweight pattern - https://en.wikipedia.org/wiki/Flyweight_pattern

# **Flyweight Pattern [Gamma95]**

**Flyweight** is a structural design pattern that allows you to work efficiently with a large number of objects, sharing the overall state between them instead of storing the same data in each object. This reduces memory consumption and improves performance.

## **When to use Flyweight?**
1. **When many objects are created in the application** and they consume too much memory.
2. ** When most states of objects can be made extrinsic and placed in a separate context.
3. **When the application does not depend on the identity of objects**, and it is possible to replace several objects with one if they have the same state.

---

## **The main components of Flyweight**
1. **Flyweight (Adaptable)**
is an interface or abstract class that defines methods that can receive an extrinsic state.
   - Example: `Tree` with methods `draw(x, y, color)'.

2. **ConcreteFlyweight (Specific Fitter)**
Is a Flyweight implementation that stores an **internal state** (intrinsic state) that does not change.
   - Example: `PineTree', `OakTree' are different types of trees, but with common characteristics (shape, texture).

3. **FlyweightFactory (Factory of adaptors)**
- Creates and manages Flyweight objects, ensuring their reuse.
   - Example: `TreeFactory.getTree(type)` returns an existing object or creates a new one.

4. **Client**
- Stores **the external state** (extrinsic state) and passes it to Flyweight objects when their methods are called.
   - Example: `Forest` draws trees by transmitting coordinates and color.

---

## **Implementation example (Java)**
Consider the example of trees in a forest, where:
- **Internal state (intrinsic)**: wood type, texture.
- **External condition (extrinsic)**: coordinates (x, y), color.

### **1. Flyweight Interface**
```java
public interface Tree {
    void draw(int x, int y, String color);
}
```

### **2. Specific Flyweight**
```java
public class ConcreteTree implements Tree {
    private final String type; // internal state (immutable)
private final String texture;

    public ConcreteTree(String type, String texture) {
        this.type = type;
        this.texture = texture;
    }

    @Override
    public void draw(int x, int y, String color) {
        System.out.printf("Draw %s tree (%s) in (%d, %d) color %s%n", 
            type, texture, x, y, color);
    }
}
```

### **3. Flyweight Factory**
```java
import java.util.HashMap;
import java.util.Map;

public class TreeFactory {
    private static final Map<String, Tree> treeCache = new HashMap<>();

    public static Tree getTree(String type) {
        Tree tree = treeCache.get(type);
        
        if (tree == null) {
// Assume that the texture is loaded from a file/database
            String texture = loadTextureForType(type);
            tree = new ConcreteTree(type, texture);
            treeCache.put(type, tree);
            System.out.println("A new Flyweight has been created: " + type);
        }
        
        return tree;
    }

    private static String loadTextureForType(String type) {
        return "texture_" + type + ".png";
    }
}
```

### **4. Client (Forest)**
``java
import java.util.ArrayList;
import java.util.List;

public class Forest {
    private final List<Tree> trees = new ArrayList<>();

    public void plantTree(int x, int y, String type, String color) {
        Tree tree = TreeFactory.getTree(type); // getting or creating Flyweight
        tree.draw(x, y, color); // passing the external state
        trees.add(tree);
    }

    public void drawForest() {
        for (Tree tree : trees) {
            // In reality, there may be complex rendering logic here
            System.out.println("The tree is drawn");
        }
    }
}
```

### **5. Using**
```java
public class Main {
    public static void main(String[] args) {
        Forest forest = new Forest();
        
        forest.plantTree(10, 20, "Oak", "Green");
        forest.plantTree(30, 40, "Pine", "Dark green");
        forest.plantTree(50, 60, "Oak", "Light Green"); // Flyweight reuse
        
        forest.drawForest();
    }
}
```

**Output:**
``
A new Flyweight has been created: Oak
A new Flyweight has been created: Pine
Draw an Oak tree (texture_Dub.png) in (10, 20)
, Draw a Pine tree (texture) in Green_Pine.png) in (30, 40) Dark green color
We draw an Oak tree (texture_Dub.png) in (50, 60) Light green color
The tree is drawn
The tree is drawn
The tree is drawn
```

---

## **Advantages of Flyweight**
✅ **Save memory** – The total state is stored once.  
***Reducing GC load** – fewer objects in the heap.  
✅ **Performance Improvement** – no need to create the same objects.  

## **Disadvantages of Flyweight**
***Code complication** – you need to separate the internal and external state.  
❌ **Potential problems with multithreading** – if Flyweight is mutable (but it is usually immutable).  

---

## **Where does Flyweight apply?**
- **Graphics engines** (games, CAD systems) – trees, bullets, particles.  
- **Text editors** – characters with common formatting.  
- **Caching** – for example, a pool of database connections.  

**Examples in Java:**
- `Integer.valueOf(int)` – caches numbers from -128 to 127.
- `String` pool of strings (String interning).  

---

## **Conclusion**
Flyweight is useful when you need to **optimize work with a large number of objects** that have a common state. It helps to reduce memory usage by separating data, but requires careful design.

 