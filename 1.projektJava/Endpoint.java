import java.io.*;
import java.net.ServerSocket;

/**
 * Class, which represeting all endpoints
 * 1) /hostname         - returns the computer's network name, including the domain
 * 2) /cpu-name         - returns information about the processor
 * 3) /load             - returns current CPU usage
 * 4) /load?refresh=X   - return current CPU usage with refreshing page X seconds
 * @author Maroš Orsák
 * created on 2018/02/09
 */
public class Endpoint {

    private String path;
    private String jsonOrPlainString;
    private String content;

    public Endpoint() { }

    /**
     * Method, endpoint /hostname
     * @param acceptLine accept header line
     * @param serverSocket socket of the server
     * @param bufferedWriter buffer
     * @throws IOException exception
     */
    public void endpointHOSTNAME(String acceptLine, ServerSocket serverSocket, BufferedWriter bufferedWriter) throws IOException {
        if(hasAcceptHeaderJson(acceptLine)){
            this.jsonOrPlainString = "{\"hostname\": \""+ serverSocket.getInetAddress().getHostName()+ "\"}";
            this.content = "application/json";
        }
        else {
            this.jsonOrPlainString = serverSocket.getInetAddress().getHostName();
            this.content = "text/plain";
        }

        bufferedWriter.write(this.jsonOrPlainString);
        bufferedWriter.flush();
    }

    /**
     * Method, endpoint /cpu-name
     * Because of two different OS, we must use different system commands
     * @param acceptLine accept header line
     * @param bufferedWriter buffer
     * @throws IOException exception
     */
    public void endpointCPUNAME(ServerSocket serverSocket, String acceptLine, BufferedWriter bufferedWriter) throws IOException {

        Process p;
        String line;
        String modelName = null;
        p = Runtime.getRuntime().exec("lscpu");
        BufferedReader br = new BufferedReader(new InputStreamReader(p.getInputStream()));
        while ((line = br.readLine()) != null) {
            if (line.contains("Model name:")) {
                String[] modelNameArray = line.split(":");                          // Model name : Intel........
                modelName = modelNameArray[1].replaceAll("^\\s+", "");
            }
        }

        if(hasAcceptHeaderJson(acceptLine)){
            jsonOrPlainString = "{\"Model name\": \"" + modelName +"\"}";
            content = "application/json";
        }
        // returning text/plain
        else {
            jsonOrPlainString = modelName;
            content = "text/plain";
        }

        bufferedWriter.write(jsonOrPlainString + System.getProperty("line.separator"));
        bufferedWriter.flush();
    }

    /**
     * Method, endpoint /load
     * @param acceptLine accept header line
     * @param bufferedWriter buffer
     * @throws IOException exception
     */
    public void endpointLOAD(String acceptLine, BufferedWriter bufferedWriter) throws IOException {
        String line;
        String[] arrayOfCpuAttributes;

        BufferedReader bufferedReader = new BufferedReader(new FileReader("/proc/stat"));
        line = bufferedReader.readLine();
        arrayOfCpuAttributes = line.split(" ");

        Long cpuUsage = CalculatorOfCPU.calculateCpuUsage(arrayOfCpuAttributes);

        fillContentTypeAndReturnedString(acceptLine, cpuUsage);

        bufferedWriter.write(jsonOrPlainString);
        bufferedWriter.flush();
    }

    /**
     * Method, endpoint /load?refresh=X
     * @param acceptLine accept header line
     * @param bufferedWriter buffer
     * @throws IOException
     */
    public void endpointLOADWITHPARAMETER(String acceptLine,  BufferedWriter bufferedWriter) throws IOException {
        String line;
        String[] arrayOfCpuAttributes;

        BufferedReader bufferedReader = new BufferedReader(new FileReader("/proc/stat"));
        line = bufferedReader.readLine();
        arrayOfCpuAttributes = line.split(" ");

        Long cpuUsage = CalculatorOfCPU.calculateCpuUsage(arrayOfCpuAttributes);

        fillContentTypeAndReturnedString(acceptLine, cpuUsage);

        bufferedWriter.write(jsonOrPlainString);
        bufferedWriter.flush();
    }

    /**
     * Method, which checked if accept header contains application/json
     * if not text/plain is returned
     * @param acceptLine accept header line
     * @return  true if application contains application/json
     *          false if does not
     */
    private boolean hasAcceptHeaderJson(String acceptLine){
        return  acceptLine.contains("application/json");
    }

    /**
     * Method, which firstly checks if accept header contains application/json and then fills returned string which will
     * receive user and content in which type the user will get that response
     * @param acceptLine accept header line
     * @param cpuUsage  current processor load
     */
    private void fillContentTypeAndReturnedString(String acceptLine, Long cpuUsage){
        if(hasAcceptHeaderJson(acceptLine)){
            jsonOrPlainString = "{\"cpu-usage\": \"" + cpuUsage +"%\"}";
            content = "application/json";
        }
        else {
            jsonOrPlainString = cpuUsage + "%";
            content = "text/plain";
        }
    }


    public String getPath() {
        return path;
    }
    public void setPath(String path) {
        this.path = path;
    }
    public String getContent() {
        return content;
    }
}
