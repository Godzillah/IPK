/**
 * Class, which has all error types
 * @author Maroš Orsák
 * created on 2018/02/04
 */
public enum ErrorTypes {

    // ARGUMENT ERROR TYPES
    BAD_ARGUMENTS("BAD_ARGUMENTS"),
    BAD_PORT("BAD_PORT"),
    NO_SPECIFIED_PORT("NO_SPECIFIED_PORT");

    private final String errorText;

    /**
     * @param errorText concrete error type
     */
    ErrorTypes(final String errorText) {
        this.errorText = errorText;
    }

    @Override
    public String toString() {
        return errorText;
    }
}
