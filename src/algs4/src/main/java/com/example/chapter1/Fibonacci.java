import java.util.*;
import java.math.BigInteger;

public class Fibonacci
{
    private static Map<Integer, BigInteger> cache = new HashMap();
    private static BigInteger big0 = new BigInteger("0");
    private static BigInteger big1 = new BigInteger("1");

    public static long F(int n)
    {
        if(n == 0) return 0;
        if (n == 1) return 1;
        return F(n-1) + F(n-2);
    }

    public static BigInteger F2(int n)
    {
        if (n == 0) return big0;
        if (n == 1) return big1;
        BigInteger v = cache.get(n);
        if (v != null) {
            return v;
        }
        BigInteger result = F2(n-1).add(F2(n-2));
        cache.put(n, result);
        return result;
    }

    public static void main(String[] args)
    {
        for (int i = 0; i < 200; i++)
        {
            System.out.println(i + " --> " + F2(i).toString());
        }
    }
}