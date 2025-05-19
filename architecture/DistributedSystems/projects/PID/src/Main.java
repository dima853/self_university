public class Main {
    public static void main(String[] args) {
        Room room = new Room(20); // the initial temperature is 20
        Thermostat thermostat = new Thermostat(25, room); // target

        for (int i = 0; i < 20; i++) {
            thermostat.regulate();
            System.out.printf("Шаг %d: Температура = %.2f°C%n",
                    i, room.getCurrentTemp());
        }
    }
}