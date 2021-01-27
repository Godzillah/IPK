import java.io.*;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;


/**
 * Class server implementing Runnable interface for the run() method, which representing clients
 * One thread <-> One client
 * @author Maroš Orsák
 * created on 2018/02/04
 */

public class Server implements Runnable {

    private final String RESPONSE_FILE = "output.txt";
    private static InetAddress InetAddr;
    private static Logger logger = MyLogger.getInstance();
    private static Integer PORT_NUMBER;
    private static ServerSocket serverSocket;
    private Socket clientSocket;

    public Server(Socket clientSocket) {
        this.clientSocket = clientSocket;
    }

    /**
     * Main method, which doing whole job of the application
     * @param args input user arguments
     * @throws IOException exception
     */
    public static void main(String[] args) throws IOException {
        // getting instance of Logger
        logger = MyLogger.getInstance();
        MyLogger.loggerInit();
        MyLogger.noStdoutWrite();                           // if you want logging info to stdout then you can comment this line

        // parse the arguments
        PORT_NUMBER = ParseArgs.parseArgs(args, logger);
        if(PORT_NUMBER == null){
            return;                                         // if port is not specified
        }

        InetAddr = InetAddress.getLocalHost();
        serverSocket = new ServerSocket(PORT_NUMBER, 50,   InetAddr);
        logger.log(Level.INFO, "Starting server on the port " + PORT_NUMBER);

        while(true){
            Server server = new Server(serverSocket.accept());

            Thread thread = new Thread(server);
            thread.start();
        }
    }

    /**
     * The core thread method
     * Each client with requests f.e /cpu-name will get response with type text/plain or application/json but they must
     * specify in the accept header
     * API only for GET method
     */
    @Override
    public void run() {
        Endpoint endpoint = new Endpoint();
        String startLine;
        String acceptLine;
        String keepAliveLine;
        File file = new File("", RESPONSE_FILE);
        int fileLength = 0;


        try (BufferedReader headersFromClient = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
             BufferedWriter headersDataOutput = new BufferedWriter(new OutputStreamWriter(clientSocket.getOutputStream()));
             BufferedOutputStream binRequestedDataOutput = new BufferedOutputStream(clientSocket.getOutputStream())){

            logger.info( "Client from: " + clientSocket.getInetAddress().getHostAddress() + " connected");

            startLine = headersFromClient.readLine();
            String[] methodAndFileRequest =  startLine.split(" ");
            String method = methodAndFileRequest[0];                // we get the HTTP method of the client
            endpoint.setPath(methodAndFileRequest[1]);              // concrete path from user f.e  /  |  /cpu-name  | etc.
            // this is for checking accept...
            while((acceptLine = headersFromClient.readLine()) != null){
                if(acceptLine.contains("Accept")){
                    break;
                }
            }
            // this is for checking keep-alive
            while((keepAliveLine = headersFromClient.readLine()) != null){
                if(keepAliveLine.contains("Keep-Alive")){
                    break;
                }
            }

            logger.info( "Accept header:" + acceptLine);

            // GET method
            if(method.equals("GET")){
                logger.info("METHOD |GET| USED WITH ENDPOINT:" + endpoint.getPath());
                // creating file for response to the client
                BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter(RESPONSE_FILE));

                if(endpoint.getPath().startsWith("/")){
                    if(endpoint.getPath().matches("^/hostname$")){
                        endpoint.endpointHOSTNAME(acceptLine, serverSocket, bufferedWriter);
                        // reads number of bytes...
                        fileLength = readNumberOfBytesInOneLine(file);
                        sendHttpHeadersWith200(headersDataOutput, endpoint.getContent(), fileLength);
                    }
                    else if (endpoint.getPath().matches("^/cpu-name$")){
                        // returning JSON
                        endpoint.endpointCPUNAME(serverSocket, acceptLine, bufferedWriter);
                        fileLength = readNumberOfBytesInOneLine(file);
                        sendHttpHeadersWith200(headersDataOutput, endpoint.getContent(), fileLength);
                    }
                    else if (endpoint.getPath().matches("^/load$")){
                        endpoint.endpointLOAD(acceptLine, bufferedWriter);
                        fileLength = readNumberOfBytesInOneLine(file);
                        sendHttpHeadersWith200(headersDataOutput, endpoint.getContent(), fileLength);
                    }
                    else if(endpoint.getPath().matches("^/load\\?refresh=[1-9][0-9]*")){
                        String[] afterEqualsSignArray = endpoint.getPath().split("=");

                        endpoint.endpointLOADWITHPARAMETER(acceptLine, bufferedWriter);
                        fileLength = (int) file.length();
                        sendHttpHeadersPlusRefreshWith200(headersDataOutput, endpoint.getContent(), fileLength,
                                Integer.parseInt(afterEqualsSignArray[1]));
                    }
                    // 400 BAD REQUEST
                    else {
                        sendHttpHeadersWith400(headersDataOutput);
                    }
                }
                logger.info("File" + endpoint.getPath() + " of type " + endpoint.getContent() + " returned");
            }
            // other methods
            else {
                // 405 Method Not Allowed
                sendHttpHeadersWith405(headersDataOutput);

            }
            byte[] binFileData = readFileBytes(file, fileLength);
            logger.info("Sending" + fileLength + " B to the " + clientSocket.getInetAddress().getHostAddress());
            binRequestedDataOutput.write(binFileData, 0, fileLength);
            binRequestedDataOutput.flush();

        } catch (IOException  e) {
            logger.log(Level.WARNING, "IOException:" + e.getMessage());
        }

        logger.info("Connection closed\n");
    }

    /**
     * Method, which takes bin.data of file and reads up to bytes of data from this input stream.
     * @param file concrete file (bin.data)
     * @param fileLength number of bytes inside file
     * @return array of bytes
     * @throws IOException exception
     */
    private byte[] readFileBytes(File file, int fileLength) throws IOException {
        byte[] fileData = new byte[fileLength];

        try (BufferedInputStream bufferedInputStream = new BufferedInputStream(new FileInputStream(file))) {
            bufferedInputStream.read(fileData);
        }
        return fileData;
    }

    /**
     * Method, which send http headers to the client with response OK - 200
     * @param output response to the client
     * @param content content-type -> text/plain or application/json
     * @param fileLength number of bytes in file
     * @throws IOException exception
     */
    private void sendHttpHeadersWith200(BufferedWriter output, String content, int fileLength) throws IOException {
        output.write("HTTP/1.1 200 OK\n");
        output.write("Server: Java HTTP server |XORSAK02|\n");
        output.write("Date: " + new Date() + "\n");
        output.write("Content-type: " + content + "\n");
        output.write("Content-length: " + fileLength + "\n");
        output.write("Keep-Alive: timeout=10, max=20\n");
        output.write("\n");
        output.flush();
    }

    /**
     * Method, which send http headers to the client with response OK - 200
     * @param output response to the client
     * @param content content-type -> text/plain or application/json
     * @param fileLength number of bytes in file
     * @throws IOException exception
     */
    private void sendHttpHeadersPlusRefreshWith200(BufferedWriter output, String content, int fileLength, int refreshDelay) throws IOException {
        output.write("HTTP/1.1 200 OK\n");
        output.write("Server: Java HTTP server |XORSAK02|\n");
        output.write("Date: " + new Date() + "\n");
        output.write("Content-type: " + content + "\n");
        output.write("Content-length: " + fileLength + "\n");
        output.write("Refresh: " + refreshDelay + "; url=http://" + serverSocket.getInetAddress().getHostAddress() +
                ":" + serverSocket.getLocalPort() + "/load?refresh="+ refreshDelay + "\n");
        output.write("\n");
        output.flush();
    }

    /**
     * Method, which send http headers to the client with response BAD REQUEST - 400
     * @param output response to the client
     * @throws IOException exception
     */
    private void sendHttpHeadersWith400(BufferedWriter output) throws IOException {
        output.write("HTTP/1.1 400 BAD REQUEST\n");
        output.write("Server: Java HTTP server |XORSAK02|\n");
        output.write("Date: " + new Date() + "\n");
        output.write("Content-type: text/plain  \n");
        output.write("Content-length: 0 " + "\n");
        output.write("\n");
        output.flush();
    }

    /**
     * Method, which send http headers to the client with response METHOD NOT ALLOWED - 405
     * @param output response to the client
     * @throws IOException exception
     */
    private void sendHttpHeadersWith405(BufferedWriter output) throws IOException {
        output.write("HTTP/1.1 405 METHOD NOT ALLOWED\n");
        output.write("Server: Java HTTP server |XORSAK02|\n");
        output.write("Date: " + new Date() + "\n");
        output.write("Content-type: text/plain  \n");
        output.write("Content-length: 0 " + "\n");
        output.write("\n");
        output.flush();
    }

    /**
     * Method, which takes all bytes in the line and calculate that length of the current line
     * @param fileName responsing file index.html or output.txt
     * @return number of bytes on the line
     * @throws IOException exception
     */
    private int readNumberOfBytesInOneLine(File fileName) throws IOException {
        BufferedReader bufferedReader = new BufferedReader(new FileReader(fileName));
        int fileLength =  bufferedReader.readLine().length();
        return fileLength;
    }


}
