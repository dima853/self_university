public class Room {
    private double currentTemp;  // текущая температура
    private double externalNoise;  // внешние помехи (например, сквозняк)

    public Room(double initialTemp) {
        this.currentTemp = initialTemp;
        this.externalNoise = 0;
    }

    // Изменяет температуру с учётом нагрева и внешних факторов
    public void updateTemp(double heatingPower) {
        currentTemp += heatingPower * 0.1;  // эффект обогрева
        currentTemp += externalNoise;       // влияние внешних помех
        externalNoise = (Math.random() - 0.5) * 0.2;  // случайный шум
    }

    public double getCurrentTemp() {
        return currentTemp;
    }

    public void setExternalNoise(double noise) {
        this.externalNoise = noise;
    }
}