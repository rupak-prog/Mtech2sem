import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

class A extends Thread {
    car c;

    public A(car c) {
        this.c = c;
    }

    @Override
    public void run() {
        try {
            c.tires();
            c.painting();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

}

class B extends Thread {
    car c;

    public B(car c) {
        this.c = c;
    }

    @Override
    public void run() {
        c.chachis();
    }

}

class C extends Thread {
    car c;

    public C(car c) {
        this.c = c;
    }

    @Override
    public void run() {
        try {
            c.seats();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

}

class A1 extends Thread {
    car c;

    public A1(car c) {
        this.c = c;
    }

    public void run() {
        try {
            c.engine();
        } catch (InterruptedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }
    
}


class A2 extends Thread {
    car c;

    public A2(car c) {
        this.c = c;
    }

    public void run() {
        try {
            c.cover();
        } catch (InterruptedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }
}

class D extends Thread {
    car c;
    public D(car c) {
        this.c = c;
    }

    @Override
    public void run() {
        try {
            A1 a1 = new A1(c);
            A2 a2 = new A2(c);
            Thread t1 = new Thread(a1);
            Thread t2 = new Thread(a2);
            t2.start();
            Thread.sleep(10);
            t1.start();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}

class car{
    int a=0,b=0,c=0,d=0,e=0;
  private final Lock aLock = new ReentrantLock();
  private final Condition b1 = aLock.newCondition();
  private final Condition b2 = aLock.newCondition();
  private final Condition b3 = aLock.newCondition();
  private final Condition b4 = aLock.newCondition();
  private final Condition b5 = aLock.newCondition();

    public void chachis(){
        boolean ans = false;
        while(!ans){
            ans = aLock.tryLock();
            if(ans){
                System.out.println("chachis is locked");
                try{
                    System.out.println("B has put chachis");
                    a=1;
                    //b1.signalAll();
                } finally{
                    aLock.unlock();
                    System.out.println("chachis is unlocked");
                }
            }
        } 
    }

    public void seats() throws InterruptedException {
        boolean ans = false;
        while(!ans){
            ans = aLock.tryLock();
            if(ans){
                System.out.println("seats is locked");
                try{
                    //b1.await();
                    if(a==1){
                        System.out.println("C attached the seats");
                        b=1;
                        break;
                    }
                    //b1.signalAll();
                    //b2.signalAll();
                } finally{
                    System.out.println("seats is unlocked");
                    aLock.unlock();
                }
            }
        }
    }

    public void engine() throws InterruptedException {
        boolean ans = false;
        while(!ans){
            ans = aLock.tryLock();
            if(ans){
                System.out.println("engine is locked");
                try{
                    //b1.await();
                    if(a==1){
                        System.out.println("D put engine");
                        c=1;
                    }
                    //b1.signalAll();
                    //b3.signalAll();
                }finally{
                    System.out.println("engine is unlocked");
                    aLock.unlock();
                }
            }
        }
    }

    public void cover() throws InterruptedException{
        boolean ans = false;
        while(!ans){
            ans = aLock.tryLock();
            if(ans){
                System.out.println("cover is locked");
                try{
                    //while(b2.await() && b3.await() && b4.await());
                    //b2.await();
                    //b3.await();
                    //b4.await();
                    if(b==1 && c==1 && d==1){
                        System.out.println("D put top cover");
                        e=1;
                    }
                    //b5.signalAll();
                }finally{
                    System.out.println("cover is unlocked");
                    aLock.unlock();
                }
            }
        }
    }

    public void tires() throws InterruptedException {
        boolean ans = false;
        while(!ans){
            ans = aLock.tryLock();
            if(ans){
                System.out.println("tires is locked");
                try{
                    //b1.await();
                    if(a==1){
                        System.out.println("A put tires");
                        d=1;
                    }
                    //b4.signalAll();
                    //b1.signalAll();
                    // b5.await();
                    // System.out.println("A painted the car");
                }finally{
                    System.out.println("tires is unlocked");
                    aLock.unlock();
                }
            }
        }
    }

    public void painting() throws InterruptedException {
        boolean ans = false;
        while(!ans){
            ans = aLock.tryLock();
            System.out.println("painting is locked");
            //b5.await();
            if(e==1){
                System.out.println("A painted the car");
            }
            System.out.println("painting is unlocked");
            aLock.unlock();
        }
    }
}


class newCarCompany {
  public static void main(String[] args) throws InterruptedException {
      int total = Integer.parseInt(args[0]);
      total = 1;
      car Car;
      A a;
      B b;
      C c;
      D d;
      Thread t[] = new Thread[4];
      for(int i=0; i<total; i++){
          Car = new car();
          a = new A(Car);
          b = new B(Car);
          c = new C(Car);
          d = new D(Car);
          t[0] = new Thread(a);
          t[1] = new Thread(b);
          t[2] = new Thread(c);
          t[3] = new Thread(d);
          t[0].start();
          t[1].start();
          t[2].start();
          t[3].start();
          t[0].join();
          t[1].join();
          t[2].join();
          t[3].join();
      }
      System.out.println("Tasks completed car is now ready...");
  }
}