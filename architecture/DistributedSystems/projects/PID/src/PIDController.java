public class PIDController {
    private double target;       // целевое значение (желаемая температура)
    private double integral;    // интегральная составляющая
    private double lastError;   // предыдущая ошибка

    public PIDController(double target) {
        this.target = target;
        this.integral = 0;
        this.lastError = 0;
    }

    // Вычисляет управляющее воздействие (PID-формула)
    public double calculate(double current) {
        double error = target - current;
        integral += error;
        double derivative = error - lastError;
        lastError = error;

        // Коэффициенты PID (можно настроить)
        double Kp = 0.5;  // пропорциональный
        double Ki = 0.01; // интегральный
        double Kd = 0.1;  // дифференциальный

        return Kp * error + Ki * integral + Kd * derivative;
    }
}