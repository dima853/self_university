public class PIDController {
    private double target; // target value (desired temperature)
    private double integral; // integral component
    private double lastError; // previous error

    public PIDController(double target) {
        this.target = target;
        this.integral = 0;
        this.lastError = 0;
    }

    // Calculates the control effect (PID formula)
    public double calculate(double current) {
        double error = target - current;
        integral += error;
        double derivative = error - lastError;
        lastError = error;

        // PID coefficients (can be adjusted)
        double Kp = 0.5; // proportional
        double Ki = 0.01; // integral
        double Kd = 0.1; // differential

        return Kp * error + Ki * integral + Kd * derivative;
    }
}