import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

class booth extends Thread{
    static int x=0,y=0,z=0;  //variable for counting vote for each candidate
    int time[];  // time when voter arrives
    char res[]; //response of each voter in sequence
    int count, boothid; //count of number of voter in the booth and id of booth

    public booth(int time[], char res[], int count, int boothid) {
        this.time = time;        //   <-|
        this.res = res;          //     |  Intialization block
        this.count = count;      //     |
        this.boothid = boothid;  //   <-|
    }

    //finding the winner of the election by comparing votes corrosponding to each voter
    public void getWinner() {
        String tmp="";
        if (x > y) {
            if (x > z) {
                System.out.println("candidate X is the winner");
                tmp="candidate X is the winner\n";
            } else if (z > x) {
                System.out.println("candidate Z is the winner");
                tmp="candidate Z is the winner\n";
            } else { 
                System.out.println("There is tie between X and Z");
                tmp="There is tie between X and Z\n";
            }
        } else if (y > x) {
            if (y > z) {
                System.out.println("candidate Y is the winner");
                tmp="candidate Y is the winner\n";
            } else if (z > y) {
                System.out.println("candidate Z is the winner");
                tmp="candidate Z is the winner\n";
            } else {
                System.out.println("There is tie between Z and Y");
                tmp="There is tie between Z and Y\n";
            }
        } else {
            if (x > z) {
                System.out.println("There is tie between Z and X");
                tmp="There is tie between Z and X\n";
            } else if (z > x) {
                System.out.println("candidate Z is the winner");
                tmp="candidate Z is the winner\n";
            } else {
                System.out.println("There is tie between All the candidates");
                tmp="There is tie between All the candidates\n";
            }
        }
        try {
            //writing winner name in file
            FileWriter fw = new FileWriter("out_file.txt",true);
            fw.write(tmp);
            fw.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    //implemented using cyclicbarrier
    public void run() {
        
        for (int i = 0; i < voting2.globalcount; i++) {
            if(i>=count){    //all the voter gave responses and no voter remaining
                //do nothing
            }else if (time[i] <= voting2.time) {  //  voter is available at current voting time
                if (res[i] == 'X') {  
                    try {
                        FileWriter fw = new FileWriter("out_file.txt",true);
                        fw.write(boothid+" chooses x\n");
                        fw.close();
                    } catch (IOException e1) {
                        e1.printStackTrace();
                    }
                    //line = boothid+" chooses x\n";
                    System.out.println(boothid+" chooses x");
                    booth.x++;              //increment x if X is selected by voter
                    
                } else if (res[i] == 'Y') {
                    //line = boothid+" chooses y\n";
                    try {
                        FileWriter fw = new FileWriter("out_file.txt",true);
                        fw.write(boothid+" chooses y\n");
                        fw.close();
                    } catch (IOException e1) {
                        e1.printStackTrace();
                    }
                    System.out.println(boothid+" chooses y");
                    booth.y++;               //increment y if Y is selected by voter
                } else {
                    try {
                        FileWriter fw = new FileWriter("out_file.txt",true);
                        fw.write(boothid+" chooses z\n");
                        fw.close();
                    } catch (IOException e1) {
                        e1.printStackTrace();
                    }
                    System.out.println(boothid+" chooses z");
                    booth.z++;              //increment z if Z is selected by voter
                }
            }else{
                i--;     //if no voter present at current voting time
            }

            if(voting2.newBarrier.getNumberWaiting()==4){
                try {
                    //If number of booth waiting at barrier becomes 4
                    System.out.println("time="+voting2.time+" x="+x+" y="+y+" z="+z+"\n");   //print current status
                    FileWriter fw = new FileWriter("out_file.txt",true);
                    fw.write("time="+voting2.time+" x="+x+" y="+y+" z="+z+"\n");
                    fw.close();
                } catch (Exception e) {
                    e.printStackTrace();
                }
                finally{
                    voting2.time++;                              //increment voting time
                    if(voting2.time == voting2.globalcount){     //if current voting time reaches to time when last candidate arrives
                        getWinner();                           //print the winner of the election
                    }
                }
            }
            
            try {
                voting2.newBarrier.await();        //all the threads wait at the barrier
            } catch (InterruptedException | BrokenBarrierException e) {
                e.printStackTrace();
            }
            if(voting2.time == voting2.globalcount){
                break;             //break when last voter gave vote
            }
        }
    }
}

class voting2 {
    static int time = 0;  //maintaining global time 
    public static CyclicBarrier newBarrier = new CyclicBarrier(5);
    static int globalcount = 10;  //max number of voter at each booth

    public static void main(String[] args) throws NumberFormatException, IOException, InterruptedException {
        int times[][] = new int[5][100];
        char responses[][] = new char[5][100];
        File delf= new File("out_file.txt");           //file to be delete  
        delf.delete();
        //reading file content and store in the array time and responses
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
        int max=0;
        for(int j=0; j<5; j++){            //finding the voter which comes most late
            if(max<times[j][count[j]-1]){
                max = times[j][count[j]-1];   //updating max variable
            }
        }   
        voting2.globalcount = max+1;   //set global count to max

        //crating booth object and initializing with voter time,responese and count and boothid
        booth b1 = new booth(times[0],responses[0],count[0],1);  
        booth b2 = new booth(times[1],responses[1],count[1],2);
        booth b3 = new booth(times[2],responses[2],count[2],3);
        booth b4 = new booth(times[3],responses[3],count[3],4);
        booth b5 = new booth(times[4],responses[4],count[4],5);

        //creating thread object for each booth and initializes with booth object
        Thread t1 = new Thread(b1);
        Thread t2 = new Thread(b2);
        Thread t3 = new Thread(b3);
        Thread t4 = new Thread(b4);
        Thread t5 = new Thread(b5);

        //starting the booths
        t1.start();
        t2.start();
        t3.start();
        t4.start();
        t5.start();

        //wait for the booths to complete
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
    }
}