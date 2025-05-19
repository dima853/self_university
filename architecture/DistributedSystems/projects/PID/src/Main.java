public class Main {
    public static void main(String[] args) {
        Room room = new Room(20);  // начальная температура = 20°C
        Thermostat thermostat = new Thermostat(25, room);  // цель = 25°C

        // Имитация работы системы (20 шагов)
        for (int i = 0; i < 20; i++) {
            thermostat.regulate();
            System.out.printf("Шаг %d: Температура = %.2f°C%n",
                    i, room.getCurrentTemp());
        }
    }
}