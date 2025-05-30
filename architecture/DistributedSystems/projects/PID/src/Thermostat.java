public class Thermostat {
    private PIDController controller;
    private Room room;

    public Thermostat(double targetTemp, Room room) {
        this.controller = new PIDController(targetTemp);
        this.room = room;
    }

    // Adjustment step
    public void regulate() {
        double currentTemp = room.getCurrentTemp();
        double heatingPower = controller.calculate(currentTemp);

        heatingPower = Math.max(-1.0, Math.min(1.0, heatingPower));

        room.updateTemp(heatingPower);
    }

    public Room getRoom() {
        return room;
    }
}