/**
 * Class, which is the core for calculate CPU-usage of PC, inspiring from Paul Colby bash script. Added link in info tag.
 * @info http://colby.id.au/calculating-cpu-usage-from-proc-stat/
 * @author Maroš Orsák
 * created on 2018/02/10
 */
public class CalculatorOfCPU {

    private static Long previousIdle = 0L;
    private static Long previousTotal = 0L;

    public CalculatorOfCPU() { }

    /**
     * Method, which is converting string to integer, parsing from /proc/stat file
     * @param stringToConvert concrete cpu atrribute from /proc/stat file
     * @return long value of that atrribute
     */
    private static Long stringToLong(String stringToConvert){
        return Long.parseLong(stringToConvert);
    }

    /**
     * Method, which calculating cpu total time
     * [2] - User attribute
     * [3] - Nice attribute
     * [4] - System attribute
     * [5] - Idle attribute
     * [6] - IoWait attribute
     * [7] - Irq attribute
     * [8] - SoftIrq attribute
     * [9] - Steal attribute
     * [10]- Guest attribute
     * @param arrayOfCpuAttributes array of cpu attributes from file /proc/stat
     * @return total sum of these atrributes
     */
    private static Long calculateCpuTotalTime(String[] arrayOfCpuAttributes){
        return  stringToLong(arrayOfCpuAttributes[2]) + stringToLong(arrayOfCpuAttributes[3]) +
                stringToLong(arrayOfCpuAttributes[4]) + stringToLong(arrayOfCpuAttributes[5]) +
                stringToLong(arrayOfCpuAttributes[6]) + stringToLong(arrayOfCpuAttributes[7]) +
                stringToLong(arrayOfCpuAttributes[8]) + stringToLong(arrayOfCpuAttributes[9]) +
                stringToLong(arrayOfCpuAttributes[10]);
    }

    /**
     * Method, which calculating cpu usage
     * [5] - Idle attribute
     * @param arrayOfCpuAttributes array of cpu attributes from file /proc/stat
     * @return percentage of cpu usage
     */
    public static Long calculateCpuUsage(String[] arrayOfCpuAttributes){
        Long idleTime = stringToLong(arrayOfCpuAttributes[5]);         // idle time from CPU

        // calculate total CPU timeInteger
        Long totalTime = calculateCpuTotalTime(arrayOfCpuAttributes);

        // calculate the cpu usage since we last checked
        Long diffIdle = idleTime - previousIdle;
        Long diffTotal = totalTime - previousTotal;
        Long diffUsage = (1000 *(diffTotal - diffIdle) / diffTotal + 5) / 10;

        previousTotal = totalTime;
        previousIdle = idleTime;

        return diffUsage;
    }
}
