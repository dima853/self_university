package languages.java.projects.EffectiveJava.item1;
/*
 * Item 1: Consider static factory methods instead of constructors 
 * A static factory is just a static method that returns an instance of a class. 
 * This is not the same as the "Factory" pattern from GoF.
 * 
 * Note that a static factory method is not the same as the Factory Method pattern
   from Design Patterns [Gamma95]. The static factory method described in this item has no direct equivalent in Design Patterns.
 */

// bad ❌
class User {
    private String name;

    public User(String name) {
        this.name = name;
    }

    // Using:
    // User user = new User("John");
}

// good ✅
class User1 {
    private String name;

    // Private constructor
    private User1(String name) {
        this.name = name;
    }

    // Static factory
    public static User1 createWithName(String name) {
        return new User1(name);
    }

    // Using:
    // User1 user1 = User1.createWithName("John");
}
