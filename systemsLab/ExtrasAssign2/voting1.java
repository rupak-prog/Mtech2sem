import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

// class keeplog {
//     static int x = 0, y = 0, z = 0;

//     synchronized static void setlog(int data) {
//         if (data == 1) {
//             x++;
//         } else if (data == 2) {
//             y++;
//         } else if (data == 3) {
//             z++;
//         }
//     }

//     public void getWinner() {
//         if (x > y) {
//             if (x > z) {
//                 System.out.println("candidate X is the winner");
//             } else if (z > x) {
//                 System.out.println("candidate Z is the winner");
//             } else {
//                 System.out.println("There is tie between X and Z");
//             }
//         } else if (y > x) {
//             if (y > z) {
//                 System.out.println("candidate Y is the winner");
//             } else if (z > y) {
//                 System.out.println("candidate Z is the winner");
//             } else {
//                 System.out.println("There is tie between Z and Y");
//             }
//         } else {
//             if (x > z) {
//                 System.out.println("There is tie between Z and X");
//             } else if (z > x) {
//                 System.out.println("candidate Z is the winner");
//             } else {
//                 System.out.println("There is tie between All the candidates");
//             }
//         }
//     }
// }

// class controller extends Thread {
//     keeplog obj;
//     int choice, res;

//     public controller(int res) {
//         this.choice = res;
//     }

//     public void run() {
//         keeplog.setlog(choice);
//     }
// }

class booth extends Thread {
    static int x=0,y=0,z=0;
    int time[];
    char res[];
    int count, boothid;

    public booth(CyclicBarrier cb, int time[], char res[], int count, int boothid) {
        //this.cb = cb;
        this.time = time;
        this.res = res;
        this.count = count;
        this.boothid = boothid;
    }

    public void getWinner() {
        if (x > y) {
            if (x > z) {
                System.out.println("candidate X is the winner");
            } else if (z > x) {
                System.out.println("candidate Z is the winner");
            } else {
                System.out.println("There is tie between X and Z");
            }
        } else if (y > x) {
            if (y > z) {
                System.out.println("candidate Y is the winner");
            } else if (z > y) {
                System.out.println("candidate Z is the winner");
            } else {
                System.out.println("There is tie between Z and Y");
            }
        } else {
            if (x > z) {
                System.out.println("There is tie between Z and X");
            } else if (z > x) {
                System.out.println("candidate Z is the winner");
            } else {
                System.out.println("There is tie between All the candidates");
            }
        }
    }

    public void run() {
        // controller c;
        // Thread response;
    //     System.out.println("**************");
    // for(int i=0; i<count; i++){
    //     System.out.println(time[i]+" "+res[i]+"bid="+boothid);
    // }
    //System.out.println("count="+count);
        for (int i = 0; i < voting1.globalcount; i++) {
            //System.out.println(time[i]+"time"+boothid);
            if(i>=count){
                //do nothing
            }else if (time[i] <= voting1.time) {
                //int resp;
                //System.out.println("hi1");
                if (res[i] == 'X') {
                    //resp = 1;
                    booth.x++;
                } else if (res[i] == 'Y') {
                    //resp = 2;
                    booth.y++;
                } else {
                    booth.z++;
                    //resp = 3;
                }
                // c = new controller(resp);
                // response = new Thread(c);
                // response.start();
            }else{
                i--;
            }
            //System.out.println("hi");
            if(voting1.newBarrier.getNumberWaiting()==4){
                System.out.println("time="+voting1.time+" x="+x+" y="+y+" z="+z);
                voting1.time++;
                if(voting1.time == voting1.globalcount){
                    getWinner();
                }
                //System.out.println(voting1.time);
            }
            try {
                voting1.newBarrier.await();
            } catch (InterruptedException | BrokenBarrierException e) {
                e.printStackTrace();
            }
            if(voting1.time == voting1.globalcount){
                break;
            }
        }
    }
}

class voting1 {
    static int time = 0;
    public static CyclicBarrier newBarrier = new CyclicBarrier(5);
    static int globalcount = 10;

    public static void main(String[] args) throws NumberFormatException, IOException {
        int times[][] = new int[5][100];
    char responses[][] = new char[5][100];
    File file=new File("voting_input.txt");    //creates a new file instance  
    FileReader fr=new FileReader(file);   //reads the file  
    BufferedReader br=new BufferedReader(fr);  //creates a buffering character input stream  
    String line;  
    int count[] = new int[5];
    for(int i=0; i<5; i++){
        count[i] = 0;
    }
    while((line=br.readLine())!=null)  
    {  
        String[] arr = line.split(" ");
        int time = Integer.parseInt(arr[0]);
        char response = arr[1].charAt(0);
        int boothid = Integer.parseInt(arr[2]);
        switch(boothid){
            case 1:
                times[0][count[0]] = time;
                responses[0][count[0]] = response;
                count[0]++;
                break;
            case 2:
                times[1][count[1]] = time;
                responses[1][count[1]] = response;
                count[1]++;
                break;
            case 3:
                times[2][count[2]] = time;
                responses[2][count[2]] = response;
                count[2]++;
                break;
            case 4:
                times[3][count[3]] = time;
                responses[3][count[3]] = response;
                count[3]++;
                break;
            case 5:
                times[4][count[4]] = time;
                responses[4][count[4]] = response;
                count[4]++;
                break;
            default:{
                System.out.println("Wrong boothid entered");
            }
        }
    }  
    fr.close();   
    
    booth b1 = new booth(newBarrier,times[0],responses[0],count[0],1);
    booth b2 = new booth(newBarrier,times[1],responses[1],count[1],2);
    booth b3 = new booth(newBarrier,times[2],responses[2],count[2],3);
    booth b4 = new booth(newBarrier,times[3],responses[3],count[3],4);
    booth b5 = new booth(newBarrier,times[4],responses[4],count[4],5);
        Thread t1 = new Thread(b1);
        Thread t2 = new Thread(b2);
        Thread t3 = new Thread(b3);
        Thread t4 = new Thread(b4);
        Thread t5 = new Thread(b5);
        t1.start();
        t2.start();
        t3.start();
        t4.start();
        t5.start();
    }
}