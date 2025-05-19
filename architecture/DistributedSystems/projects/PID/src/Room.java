public class Room {
    private double currentTemp;  // current temperature
    private double externalNoise;   // external noise (for example, a draft))

    public Room(double initialTemp) {
        this.currentTemp = initialTemp;
        this.externalNoise = 0;
    }

    // Changes the temperature based on heating and external factors
    public void updateTemp(double heatingPower) {
        currentTemp += heatingPower * 0.1;  // heating effect
        currentTemp += externalNoise; // influence of external noise
        externalNoise = (Math.random() - 0.5) * 0.2;  // random noise
    }

    public double getCurrentTemp() {
        return currentTemp;
    }

    public void setExternalNoise(double noise) {
        this.externalNoise = noise;
    }
}