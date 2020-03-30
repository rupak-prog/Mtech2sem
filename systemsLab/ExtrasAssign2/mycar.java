// class A1 extends Thread{
//     car c;
//     String s;
//     public A1(car c,String s){
//         this.s = s;
//         this.c = c;
//     }
//     public void run(){
//         c.tires(s);
//     }
// }

// class A2 extends Thread{
//     car c;
//     String s;
//     public A2(car c,String s){
//         this.c = c;
//         this.s = s;
//     }
//     public void run(){
//         c.painting(s);
//     }
// }

class A extends Thread {
    car c;
    // Thread t1,t2;
    // A1 a1;
    // A2 a2;
    public A(car c) {
        this.c = c;
    }

    @Override
    public void run() {
        // a1 = new A1(c,Thread.currentThread().getName());
        // a2 = new A2(c,Thread.currentThread().getName());
        // t1 = new Thread(a1);
        // t2 = new Thread(a2);
        // t2.start();
        // t1.start();
        c.tires(Thread.currentThread().getName());
        c.painting(Thread.currentThread().getName());
    }
}

class B extends Thread {
    car c;

    public B(car c) {
        this.c = c;
    }

    @Override
    public void run() {
        c.chachis(Thread.currentThread().getName());
    }

}

class C extends Thread {
    car c;

    public C(car c) {
        this.c = c;
    }

    @Override
    public void run() {
        c.seats(Thread.currentThread().getName());
    }
}

class D extends Thread {
    car c;

    public D(car c) {
        this.c = c;
    }

    @Override
    public void run() {
        c.engine(Thread.currentThread().getName());
        c.cover(Thread.currentThread().getName());
    }
}

class initiate extends Thread {
    car Car;
    A a;
    B b;
    C c;
    D d;
    Thread t[];
    String s;

    public initiate(String s) {
        t = new Thread[4];
        this.s = s;
    }

    public void run() {
        Car = new car();
        a = new A(Car);
        b = new B(Car);
        c = new C(Car);
        d = new D(Car);
        t[0] = new Thread(a, s);
        t[1] = new Thread(b, s);
        t[2] = new Thread(c, s);
        t[3] = new Thread(d, s);
        t[3].start();
        t[1].start();
        t[2].start();
        t[0].start();
        try{
            t[0].join();
            t[1].join();
            t[2].join();
            t[3].join();
        }catch(InterruptedException e){
            System.out.println(e);
        }
    }

    public void waiting(){
        
    }
}

class car{
    int a=0,b=0,c=0,d=0,e=0;

    public void chachis(String s){
        boolean ans = false;
        while(!ans){
            System.out.println(s+" B has put chachis");
            a=1;
            System.out.println("hi4"+a+b+c+d+e);
            ans = true;
        } 
    }

    public void seats(String s){
        boolean ans = false;
        while(!ans){
            if(a==1){
                //System.out.println("hi");
                System.out.println(s+" C attached the seats");
                System.out.println("hi4"+a+b+c+d+e);
                b=1;
                ans = true;
                break;
            }
        }
    }

    public void engine(String s){
        boolean ans = false;
        while(!ans){
            if(a==1){
                System.out.println(s+" D put engine");
                System.out.println("hi4"+a+b+c+d+e);
                c=1;
                ans = true;
                break;
            }
        }
    }

    public void cover(String s){
        boolean ans = false;
        while(!ans){
            if(b==1 && c==1 && d==1){
                System.out.println(s+" D put top cover");
                e=1;
                System.out.println("hi4"+a+b+c+d+e);
                ans = true;
                break;
            }
        }
    }

    public void tires(String s){
        boolean ans = false;
        while(!ans){
            //System.out.println("hi3"+a+b+c+d+e);
            if(a==1){
                System.out.println(s+" A put tires");
                System.out.println("hi4"+a+b+c+d+e);
                d=1;
                ans=true;
                break;
            }
        }
    }
    

    public void painting(String s){
        boolean ans = false;
        while(!ans){
            //System.out.println("hi4"+a+b+c+d+e);
            if(e==1){
                System.out.println(s+" A painted the car");
                ans = true;
                break;
            }
        }
    }
}


class mycar {
  public static void main(String[] args) throws InterruptedException {
      int total = Integer.parseInt(args[0]);
      Thread t[] = new Thread[total];
      String s = "car";
      for(int i=0; i<total; i++){
        t[i] = new Thread(new initiate(s+Integer.toString(i+1)));
        t[i].start();
      }
      for(int i=0; i<total; i++){
          t[i].join();
          System.out.println("Car "+(i+1)+" is ready");
      }
      System.out.println("Tasks completed car is now ready...");
  }
}