package popCountInJava;

import java.util.Random;

public class main {
	
	public static int popcount_1_control(long x) {
		int c = 0; 
		for (int i = 0; i < 64; i++) {
			if ((x & 1) == 1) {
				c++;
			}
			x >>= 1; 
		}
		return c;
	}
	static int pop4[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};
	public static int popcount_4_data(long x) {
		int c = 0; 
		for (int i = 0; i < 16; i++) {
			c += pop4[(int) (x&0xf)]; 
			x >>= 4; 
		}
		return c;
	}
	public static int fast(long x) {
		return Integer.bitCount((int) x);
	}
	public static long randomLong(long seed) {
		Random rando = new Random();
		rando.setSeed(seed);
		return rando.nextLong();
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		long rand = randomLong(8);
		for (int i = 0; i < 10000; i++) {
			long randomInLoop = randomLong(i);
			fast(randomInLoop);
			popcount_4_data(randomInLoop);
			popcount_1_control(randomInLoop);
		}
		long startFast = System.nanoTime();
		fast(rand);
		long endFast = System.nanoTime();
		long timeDataFast = endFast - startFast; 
		System.out.println("The fast time in nanoseconds: " + timeDataFast);
		
		long startPop4 = System.nanoTime();
		popcount_4_data(rand);
		long endPop4 = System.nanoTime();
		long timeDataPop4 = endPop4 - startPop4;
		System.out.println("The popcount4_data time in nanoseconds: " + timeDataPop4);
		
		long startPop1 = System.nanoTime();
		popcount_1_control(rand); 
		long endPop1 = System.nanoTime();
		long timeDataPop1 = endPop1 - startPop1;
		System.out.println("The popcount_1_control time in nanoseconds: " + timeDataPop1);
	}

}
