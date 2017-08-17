public class Puzzle {
	static boolean answerReady = false;
	static int answer = 0;

	public static void main(String[] args) throws InterruptedException {


		class Thread1 extends Thread {
			public void run() {
				answer = 42;
				answerReady = true;
			}
		}

		class Thread2 extends Thread {
			public void run() {
				if (answerReady) {
					System.out.println("The meaning of life is: " + answer);
				} else {
					System.out.println("I dont know the answer");
				}
			}
		}

		Thread1 t1 = new Thread1();
		Thread2 t2 = new Thread2();
		t1.start();
		t2.start();
		//Thread.yield();
		t1.join();
		t2.join();
	} 
}