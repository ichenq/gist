import java.io.*;
import java.util.*;
import java.lang.reflect.*;

public class ClassPrinter {
    private static PrintStream stream = System.out;

    public static void InitStream(PrintStream strm)
    {
        stream = strm;
    }

    public static void printClassProtoType(String clsName)
            throws ReflectiveOperationException
    {
        StringBuilder sb = new StringBuilder();
        sb.append("class ");
        Class cls = Class.forName(clsName);
        String modifier = Modifier.toString(cls.getModifiers());
        if (modifier.length() > 0) {
            sb.append(modifier);
        }
        sb.append(cls.getName());
        Class supercls = cls.getSuperclass();
        if (supercls != null && supercls != Object.class) {
            sb.append(" : ");
            sb.append(supercls.getName());
        }

        sb.append("\n");
        sb.append("{\n");
        printConstructors(sb, cls);
        sb.append("\n");
        printMethods(sb, cls);
        sb.append("\n");
        printFields(sb, cls);
        sb.append("\n}\n");
        stream.println(sb.toString());
    }

    public static void printConstructors(StringBuilder sb, Class cls)
    {
        Constructor[] ctors = cls.getDeclaredConstructors();
        for (Constructor ctor : ctors)
        {
            sb.append("    ");
            String modifier = Modifier.toString(ctor.getModifiers());
            sb.append(modifier);
            sb.append(" ");
            sb.append(ctor.getName());
            sb.append("(");
            Class[] paramTypes = ctor.getParameterTypes();
            for (int i = 0; i < paramTypes.length; i++)
            {
                Class paramType = paramTypes[i];
                if (i > 0) {
                    sb.append(", ");
                }
                sb.append(paramType.getName());
            }
            sb.append(");\n");
        }
    }

    public static void printMethods(StringBuilder sb, Class cls)
    {
        Method[] methods = cls.getDeclaredMethods();
        for (Method m : methods)
        {
            sb.append("    ");
            String modifier = Modifier.toString(m.getModifiers());
            if (modifier.length() > 0) {
                sb.append(modifier);
                sb.append(" ");
            }
            sb.append(m.getReturnType().getName());
            sb.append(" ");
            sb.append(m.getName());
            sb.append("(");
            Class[] paramTypes = m.getParameterTypes();
            for (int i = 0; i < paramTypes.length; i++)
            {
                Class paramType = paramTypes[i];
                if (i > 0) {
                    sb.append(", ");
                }
                sb.append(paramType.getName());
            }
            sb.append(");\n");
        }
    }

    public static void printFields(StringBuilder sb, Class cls)
    {
        Field[] fields = cls.getDeclaredFields();
        for (Field field : fields)
        {
            sb.append("    ");
            String modifier = Modifier.toString(field.getModifiers());
            if (!modifier.isEmpty()) {
                sb.append(modifier);
                sb.append(" ");
            }
            sb.append(field.getType().getName());
            sb.append(" ");
            sb.append(field.getName());
            sb.append("\n");
        }
    }
}
