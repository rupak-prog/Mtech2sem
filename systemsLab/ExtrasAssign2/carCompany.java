import java.util.concurrent.locks.ReentrantLock; 

class A implements Runnable {
    ReentrantLock l1,l2,l3,l4,l5;
    public A(ReentrantLock l1,ReentrantLock l2,ReentrantLock l3,ReentrantLock l4,ReentrantLock l5){
        this.l1 = l1;
        this.l2 = l2;
        this.l3 = l3;
        this.l4 = l4;
        this.l5 = l5;
      }
    public void run() {
      if(l1.tryLock()){
        System.out.println("A added tires."); // 4
        //l4.unlock();
      }
      if(l5.tryLock()){
        System.out.println("A painted the car."); // 6
      }
  }
}

class B implements Runnable {
    ReentrantLock l1,l2,l3,l4,l5;
    public B(ReentrantLock l1,ReentrantLock l2,ReentrantLock l3,ReentrantLock l4,ReentrantLock l5){
        this.l1 = l1;
        this.l2 = l2;
        this.l3 = l3;
        this.l4 = l4;
        this.l5 = l5;
      }
  public void run() {
      System.out.println("B put chassis on the belt.");
      //l1.unlock();
  }
}

class C implements Runnable {
    ReentrantLock l1,l2,l3,l4;
  public C(ReentrantLock l1,ReentrantLock l2,ReentrantLock l3,ReentrantLock l4,ReentrantLock l5){
    this.l1 = l1;
        this.l2 = l2;
        this.l3 = l3;
        this.l4 = l4;
  }
  public void run() {
      if(l1.tryLock()){
        System.out.println("C attached the seat."); // 2
        //l2.unlock();
      }
  }
}

class D implements Runnable {
    ReentrantLock l1,l2,l3,l4,l5;
    public D(ReentrantLock l1,ReentrantLock l2,ReentrantLock l3,ReentrantLock l4,ReentrantLock l5){
        this.l1 = l1;
        this.l2 = l2;
        this.l3 = l3;
        this.l4 = l4;
        this.l5 = l5;
  }
  public void run() {
      if(l1.tryLock()){
        System.out.println("D put engine."); // 3
        //l3.unlock();
      }
      if(l2.tryLock() && l3.tryLock() && l4.tryLock()){
        l5.lock();
        System.out.println("D put top cover."); // 5
        l5.unlock();
      }
  }
}

class car implements Runnable{
    private static ReentrantLock lock1 = new ReentrantLock();
    private static ReentrantLock lock2 = new ReentrantLock();
    private static ReentrantLock lock3 = new ReentrantLock();
    private static ReentrantLock lock4 = new ReentrantLock();
    private static ReentrantLock lock5 = new ReentrantLock();
    public void run(){
      A a = new A(lock1,lock2,lock3,lock4,lock5);
      B b = new B(lock1,lock2,lock3,lock4,lock5);
      C c = new C(lock1,lock2,lock3,lock4,lock5);
      D d = new D(lock1,lock2,lock3,lock4,lock5);
      Thread t1 = new Thread(a);
      Thread t2 = new Thread(b);
      Thread t3 = new Thread(c);
      Thread t4 = new Thread(d);
      lock1.lock();
      lock2.lock();
      lock3.lock();
      lock4.lock();
      lock5.lock();
      t2.start();
      lock1.unlock();
      t1.start();
      t3.start();
      t4.start();
      try {
          t1.join();
      } catch (InterruptedException e) {
          e.printStackTrace();
      }
  }
}

class carCompany {
  public static void main(String[] args) throws InterruptedException {
      int count = 0, total = Integer.parseInt(args[0]);
      car c;
      total = 1;
      Thread t[] = new Thread[total];
      for(int i=0; i<total; i++){
          c = new car();
          t[i] = new Thread(c);
          t[i].start();
      }
      for(int i=0; i<total; i++){
          t[i].join();
          count++;
          System.out.println("car "+ count +" is ready");
      }
      System.out.println("Tasks completed car is now ready...");
  }
}