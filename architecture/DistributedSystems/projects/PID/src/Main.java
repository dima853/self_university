public static void main(String[] args) {
    Room room = new Room(20);
    Thermostat thermostat = new Thermostat(25, room);
    double precision = 0.1; //

    for (int i = 0; ; i++) {
        thermostat.regulate();
        System.out.printf("Шаг %d: Температура = %.2f°C%n", i, room.getCurrentTemp());

        if (Math.abs(room.getCurrentTemp() - 25) < precision) {
            System.out.println("Цель достигнута!");
            break;
        }
    }
}