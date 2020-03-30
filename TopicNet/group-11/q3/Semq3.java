import java.io.File;
import java.io.FileWriter;
import java.util.Random;
import java.util.Scanner;
import java.util.concurrent.Semaphore;

class Semq3 {
    // three services are provided by bank

    // the volatile variable "ai" keeps track of 
    // services provided by bak randomly
    static volatile int ai = 0, cid=0;
    static String[] services = { "Draft Processing", "Cheque Processing", "Net Banking" };
    public static void main(String[] args) {
        try {
            File log = new File("logfile.txt"); 
            log.delete();
            // user gives the number of customer arrived at bank
            final Scanner in = new Scanner(System.in);
            final int cust = in.nextInt();
            in.close();
            if(cust<=0){
                System.out.println("No customers today.");
                return;
            }
            FileWriter pen = new FileWriter("logfile.txt", true);
            System.out.println("Cust_Id    Bank Services    Voucher_Id\n---------------------------------------");
            pen.write("Cust_Id    Bank Services    Voucher_Id\n---------------------------------------\n");
            pen.close();
            CThreads[] customers = new CThreads[cust];
            Thread[] td = new Thread[cust];
            Semaphore semp = new Semaphore(1);

            // each customer is associated with a thread
            for (int i = 0; i < cust; i++) {
                customers[i] = new CThreads(new Random().nextInt(3), semp);
                td[i] = new Thread(customers[i]);
                td[i].start();
            }

            while(cid < cust){
                ai = new Random().nextInt(3);
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
        
    }


    // threads are created by extending the Thread class
    // syncronization is achieved by using Semaphore class
    // Semaphore class method's acquire() and release()
    // are used to execute critical section without interference
    static class CThreads extends Thread {
        int service_num;
        Semaphore sem;

        public CThreads(int s, Semaphore sp) {
            this.service_num = s;
            this.sem = sp;
        }

        // Thread's class run() method is overrided for thread execution
        @Override
        public void run() {
            while (ai != service_num);
            try {
                sem.acquire();
                cid=cid+1;
                FileWriter pen = new FileWriter("logfile.txt", true);
                System.out.format("%4s %20s %8s\n",cid,services[ai],(10000+cid));
                pen.write(String.format("%4s %20s %8s\n",cid,services[ai],(10000+cid)));
                pen.close();
                
            } catch (Exception e) {
                e.printStackTrace();
            }
            finally{
                sem.release();
            }
            
        }
    }

}

