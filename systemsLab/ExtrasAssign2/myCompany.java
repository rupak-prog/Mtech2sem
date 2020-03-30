class A implements Runnable {
    public void run() {
        try {
            Thread.sleep(300);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("A added tires."); // 4
        try {
            Thread.sleep(500);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("A painted the car."); // 6
    }
}

class B implements Runnable {
    public void run() {
        System.out.println("B put chassis on the belt.");
    }
}

class C implements Runnable {
    public void run() {
        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("C attached the seat."); // 2
    }
}

class D implements Runnable {
    public void run() {
        try {
            Thread.sleep(200);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("D put engine."); // 3
        try {
            Thread.sleep(400);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("D put top cover."); // 5
    }
}

class car implements Runnable{
    public void run(){
        A a = new A();
        B b = new B();
        C c = new C();
        D d = new D();
        Thread t1 = new Thread(a);
        Thread t2 = new Thread(b);
        Thread t3 = new Thread(c);
        Thread t4 = new Thread(d);
        t1.start();
        t2.start();
        t3.start();
        t4.start();
        try {
            t1.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}

class myCompany {
    public static void main(String[] args) throws InterruptedException {
        int count = 0, total = Integer.parseInt(args[0]);
        car c = new car();
        for(int i=0; i<total; i++){
            Thread t = new Thread(c);
            t.start();
            t.join();
            count++;
            System.out.println("car "+ count +" is ready");
        }
        System.out.println("Tasks completed car is now ready...");
    }
}