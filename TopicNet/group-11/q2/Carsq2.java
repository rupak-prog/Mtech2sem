import java.util.Scanner;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Carsq2 {
    // these static variables store the number of parts to be attached separately
    static int chasisnum, seatnum, tirenum, enginenum, topnum, paintnum, num;
    // ReentrantLock is used to provide synchronization among workers
    static Lock tala = new ReentrantLock();
    
    public static void main(String[] args) {
        // taking number of cars to be manufactured from user
        Scanner in = new Scanner(System.in);
        int numcar = in.nextInt();
        in.close();
        if(numcar<=0){
            System.out.println("No cars to manufacture today.");
            return;
        }
        num = numcar;
        chasisnum=numcar; seatnum=numcar; tirenum=numcar;
        enginenum=numcar; topnum=numcar; paintnum=numcar;
        // initializing threads
        Thread ta = new Thread(new TechnicianA());
        Thread tb = new Thread(new TechnicianB());
        Thread tc = new Thread(new TechnicianC());
        Thread td = new Thread(new TechnicianD());
        // running threads
        ta.start();
        tb.start();
        tc.start();
        td.start();

    }

    // threads are created by extending the Thread class
    // and they are using reentrant lock's lock() 
    // and unlock() method for achieving synchronization

    static class TechnicianA extends Thread{
        // technician A is allowed to fit tire and paint the car
        int cn, en, sn, trn, tpn, pn;
        @Override
        public void run() {
            while (paintnum>0){ 
                tala.lock();
                //System.out.println("A tires start");
                try {
                    trn=tirenum;
                    cn=chasisnum;
                    if(trn>cn){
                        System.out.println("--> A added tires.");
                        tirenum=tirenum-1;
                    }    
                } catch (Exception e) {
                    e.printStackTrace();
                }
                finally{
                    //System.out.println("A tires end");
                    tala.unlock();
                } 

                tala.lock();
                //System.out.println("A paint start");
                try {
                    pn=paintnum;
                    sn=seatnum;
                    en=enginenum;
                    tpn=topnum;
                    cn=chasisnum;
                    if(pn>cn && pn>sn && pn>en && pn>trn && pn>tpn){
                        System.out.println("--> A painted the car.");
                        paintnum=paintnum-1;
                    }                        
                } catch (Exception e) {
                    e.printStackTrace();
                }
                finally{
                    //System.out.println("A paint end");
                    tala.unlock();
                }
                
            }
        }
    }

    static class TechnicianB extends Thread{
        // technician B is only allowed to fit chasis in the car
        @Override
        public void run() {
            while(paintnum>0 && chasisnum>0){ 
                tala.lock();
                //System.out.println("B start");
                try {
                    if(chasisnum==paintnum){
                        System.out.println("Manufacturing Car "+(num-paintnum+1));
                        System.out.println("--> B put chassis on the belt.");
                        chasisnum=chasisnum-1;
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
                finally{
                    //System.out.println("B end");
                    tala.unlock();
                }
            }            
        }
    }

    static class TechnicianC extends Thread{
        // technician C should attached the seat in car
        int cn, sn, pn;
        @Override
        public void run() {
            while(paintnum>0){
                tala.lock();
                //System.out.println("C start");
                try {
                    sn=seatnum;
                    cn=chasisnum;
                    if(sn>cn){
                        System.out.println("--> C attached the seat.");
                        seatnum=seatnum-1;
                    }    
                } catch (Exception e) {
                    e.printStackTrace();
                }
                finally{
                    //System.out.println("C end");
                    tala.unlock();
                }
            }
        }
    }

    static class TechnicianD extends Thread{
        // technician D can put engine and top cover
        int cn, en, sn, trn, tpn, pn;
        @Override
        public void run() {
            while(paintnum>0){
                tala.lock();
                //System.out.println("D engine start");
                try {
                    cn=chasisnum;
                    en=enginenum;
                    if(en>cn){
                        System.out.println("--> D put engine.");
                        enginenum=enginenum-1;
                    }    
                } catch (Exception e) {
                    e.printStackTrace();
                }
                finally{
                    //System.out.println("D engine end");
                    tala.unlock();
                }

                tala.lock();
                //System.out.println("D top start");
                try {
                    trn=tirenum;
                    cn=chasisnum;
                    sn=seatnum;
                    en=enginenum;
                    tpn=topnum;
                    if(tpn>cn && tpn>sn && tpn>en && tpn>trn){
                        System.out.println("--> D put top cover.");
                        topnum=topnum-1;
                    }   
                } catch (Exception e) {
                    e.printStackTrace();
                }
                finally{
                    //System.out.println("D top end");
                    tala.unlock();
                } 
            }  
        }
    }

}