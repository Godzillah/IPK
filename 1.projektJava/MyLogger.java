import java.io.*;
import java.util.logging.FileHandler;
import java.util.logging.Logger;
import java.util.logging.SimpleFormatter;

/**
 * Singleton pattern to see all bad attempts and getting data to file
 * this is simple version only for one file (for production will be more files and also erasing after some period of time)
 * @author Maroš Orsák
 * created on 2018/02/04
 */
class MyLogger {

    private static final String PATH_TO_LOG_FILE = "log";
    private static FileHandler fileHandler;
    private static Logger logger = Logger.getLogger("ServerLogger");
    private static SimpleFormatter formatter;

    private MyLogger() { }

    public static void loggerInit() throws IOException {
        fileHandler = new FileHandler(PATH_TO_LOG_FILE, 16384000, 5, true);     //5 x 16MB
        logger.addHandler(fileHandler);
        formatter = new SimpleFormatter();
        fileHandler.setFormatter(formatter);
    }

    public static void noStdoutWrite(){
        logger.setUseParentHandlers(false);
    }

    /**
     * Method which returns instance of Logger
     * @return instance of logger
     */
    public static Logger getInstance(){
        return logger;
    }

}
