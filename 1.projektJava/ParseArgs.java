import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Class, which parsing input arguments from user
 * @author Maroš Orsák
 * created on 2018/02/04
 */
public class ParseArgs {

    /**
     * Static function which will take care of inputs from user
     * First method checks if user input some argument after |make run  (bad)
     * Secondly if user input | make run port=                          (bad)
     * Thirdly if user input port number from 1000 to inf...            (right)
     * @param args input arguments from user
     * @param logger instance of logger to catch all errors
     */
    public static Integer parseArgs(String[] args, Logger logger) {
        if(args[0] != null){
            String[] firstArgument = args[0].split("=");
            if(firstArgument.length != 1){                                  // array not contains port number after sing =
                if(firstArgument[1].matches("^[1-9][0-9][0-9][0-9]+$")){      //  only port number greater then 1000
                    return Integer.parseInt(firstArgument[1]);
                }
                else{
                    System.out.println("ERROR: Wrong port number must be from interval <1000, ..> ");
                    logger.log(Level.INFO, "Wrong port number must be from interval <1000, ..>", ErrorTypes.BAD_PORT);
                    Help.help();
                }
            }
            else {
                System.out.println("ERROR: Non-specified port");
                logger.log(Level.INFO, "Wrong port number must be from interval <1000, ..>", ErrorTypes.NO_SPECIFIED_PORT);
                Help.help();
            }
        }
        else{
            System.out.println("ERROR: You did no type argument ");
            logger.log(Level.INFO, "Wrong port number must be from interval <1000, ..>", ErrorTypes.BAD_ARGUMENTS);
            Help.help();
        }
        return null;
    }
}
