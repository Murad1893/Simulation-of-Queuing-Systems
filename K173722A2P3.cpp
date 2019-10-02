#include <iostream>
#include <windows.h>
#include <time.h>
#include <fstream>
using namespace std;

static int customer_count=0; //to store the customer count in order to print each customer with a different integer like "Customer 1,Customer 2..." etc

class customer{ 
	
	private:
	int arrival_hr; 
	int arrival_min;
	int mins; //this is the hrs+min in mins
	int service_time;
	int wait_time;
	
	public:
	customer(){
		service_time=0;
		wait_time=0;
	}
	
	void setarrivaltime(int min){
		arrival_hr=min/60;
		arrival_min=min%60;
		mins=min;
	}
	
	int getarrivaltime(){
		return mins;
	}
	
	int getarrivalhr(){
		return arrival_hr;
	}
	
	int getarrivalmin(){
		return arrival_min;
	}
	
	void setservicetime(int a){
		service_time=a;
	}
	
	void setwaittime(int a){
		wait_time=a;
	}
    
    int getwaittime(){
    	return wait_time;
	}
    
    int getservicetime(){
    	return service_time;
	}
	
	void print(){ //print the customer info
		cout<<"\nCustomer "<<++customer_count<<": ";
		if(arrival_hr/10==0){
			if(arrival_min/10==0)
			cout<<"\nArrived at: 0"<<arrival_hr<<":0"<<arrival_min;
			else
			cout<<"\nArrived at: 0"<<arrival_hr<<":"<<arrival_min;
		}
		else
		cout<<"\nArrived at: "<<arrival_hr<<":"<<arrival_min;
		cout<<"\nService time: "<<service_time;
		cout<<"\nWait time: "<<wait_time;
		cout<<endl<<endl;
	}
	
};

int random_number(int min,int max){ //generate random number based within a range
	return (rand())%(max-min+1)+min;
}

void simulator(int e){ //runs the simulator for the specified number of hours 
	fstream obj;
	obj.open("A2P3-out.txt",ios::out|ios::trunc);
	int end_time=e; //this is end time of the program. This can be user specified. As the POS is 18*7 hence for a particular day runs for 18 hours and so done for 18 hours
	srand(time(0)); //this is done in order to generate pseudo random numbers
	int clock_time=random_number(0,10); //clock time here denotes that at what time the customer arrives at the POS. at the start can be anytime between 0 - 10 mins after opening of the POS
	int prevclock_hr=0; //this variable assists in checking whether one hour mark has passed or not
	int i=0; //total number of customers
	int k=0; //this count the customers uptil the 1 hour mark
	bool ishour=false; //this variable assists in checking whether one hour mark has passed or not
	customer c[250]; //maximum of a 250 customer count made should be increased if the running time of the POS is greater than 18 but is fine if below equal to 24
	float average_waittime=0; //this is to calculate the avg.waiting time per hour
	float average_servicetime=0; //average service time for POS utilization
	int hour_count=0; //for statistic
	
	end_time=end_time*60; //program made in number of minutes
	
	srand(time(0));
	while(clock_time+c[i].getwaittime()<=end_time-8){ //the program terminates when the arrival time of last customer is greater the max service time + endtime in order to give him proper service before the end time of the shop 
		//Sleep(100); can be used to see simulation
		if(ishour){ //if the hour mark is reached
			average_waittime=0;
			cout<<"\n***************************************\n";
			obj<<"***************************************\n";
			cout<<"Statistics: from "<<hour_count-1<<":00"<<"-"<<++hour_count<<":00"<<"\n";
			obj<<"Statistics: from "<<hour_count-1<<":00"<<"-"<<hour_count<<":00"<<"\n";
			for(int j=k;j<=i;j++){ //loop for the customers uptil the hour mark
				average_waittime+=c[j].getwaittime(); 
			}
			if((average_waittime/(i-k))>3){ //busy hour identification
			cout<<"\nBUSY HOUR. Average waiting time > 3\n";
			obj<<"\nBUSY HOUR. Average waiting time > 3\n";
			}
			cout<<"\nAverage waiting time: "<<average_waittime/(i-k);
			obj<<"\nAverage waiting time: "<<average_waittime/(i-k)<<" mins/customer";
			cout<<"\nNumber of customers in this hour: "<<i-k;
			obj<<"\nNumber of customers in this hour: "<<i-k;
			cout<<"\n***************************************\n";
			obj<<"\n***************************************\n\n";
			cout<<"\n\n";
			k=i; //so that now k is initialized to next customer number after the hour mark
			ishour=false; //both the checking variables are reset
			prevclock_hr=0;
		}
		cout<<"\t---------------------------------------------------\n";
		cout<<"\t\t\tEND TIME: "<<end_time/60<<":0"<<end_time%60<<endl;
		if((clock_time/60)/10==0){
			if((clock_time%60)/10==0)
			cout<<"\t\t\tCLOCK:    0"<<clock_time/60<<":0"<<clock_time%60<<"\n";
			else
			cout<<"\t\t\tCLOCK:    0"<<clock_time/60<<":"<<clock_time%60<<"\n";
		}
		else
		cout<<"\t\t\tCLOCK:    "<<clock_time/60<<":"<<clock_time%60<<"\n";
		cout<<"\t---------------------------------------------------";	
		prevclock_hr=clock_time/60;	
	
	    c[i].setarrivaltime(clock_time); //the arrival time of the customer is the recent time on the clock
	    clock_time+=random_number(0,15); //this condition implies that the maximum gap between two customer arrivals can be 15 mins that another customer will come after 15 mins. 0<=Interarrival time<=15
	    
	    prevclock_hr=clock_time/60-prevclock_hr; //difference between the clock_times in hours to ensure whether an hour has passed or not
	    if(prevclock_hr==1){
	    	ishour=true;
		}
		
		c[i].setservicetime(random_number(3,8)); //random service time allocated between 3-8
		if(i==0){
	    	c[i].setwaittime(0); //if the first customer comes than he does not have to wait
		}
		else{
			//this incoorporates that wait time for the new customer is the (service+wait) time of the previous arrived customer subtracted from the difference in the arrival times of both
			int wait=(c[i-1].getwaittime()+c[i-1].getservicetime())-(c[i].getarrivaltime()-c[i-1].getarrivaltime()); 
			if(wait<=0){ //if wait time is negative means no need to wait then set to zero
				c[i].setwaittime(0);
			}
			else c[i].setwaittime(wait);
		}
		c[i].print();	
		i++;
	}
	
	average_waittime=0;
	cout<<"\n***************************************\n";
	obj<<"***************************************\n";
	cout<<"Statistics: from "<<hour_count-1<<":00"<<"-"<<++hour_count<<":00"<<"\n";
	obj<<"Statistics: from "<<hour_count-1<<":00"<<"-"<<hour_count<<":00"<<"\n";
	for(int j=k;j<=i;j++){
		average_waittime+=c[j].getwaittime();
	}
	if((average_waittime/(i-k))>3){
		cout<<"\nBUSY HOUR. Average waiting time > 3\n";
		obj<<"\nBUSY HOUR. Average waiting time > 3\n";
	}
	cout<<"\nAverage waiting time: "<<average_waittime/(i-k);
	obj<<"\nAverage waiting time: "<<average_waittime/(i-k)<<" mins/customer";
	cout<<"\nNumber of customers in this hour: "<<i-k;
	obj<<"\nNumber of customers in this hour: "<<i-k;
	for(int j=0;j<i;j++){
		average_servicetime+=c[j].getservicetime();
	}
	cout<<"\n***************************************\n";
	obj<<"\n***************************************\n\n";
	
	cout<<"\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";
	cout<<"Average POS utilization time: "<<average_servicetime/i;
	obj<<"Average POS utilization time: "<<average_servicetime/i<<" mins/customer";
	cout<<"";
	cout<<"\nTotal customers: "<<i;
	obj<<"\nTotal customers: "<<i;
	cout<<"\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";
	
}

int main(){
	
	int end_time=18; //user can input number of hours to be run from here
	simulator(end_time);

}
