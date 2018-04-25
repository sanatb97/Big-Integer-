#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "intal.h"
typedef struct bigint 
{
	int *a;
	int size;

}bigint;
/*void atoia(char *src, int *dst, int len)
{
 	int i;
 	for(i=0;i<len;i++)
 	{
 		dst[i]=src[i]-'0';
 	}
}
*/
void *intal_create(const char * str){
	bigint *p=(bigint *)malloc(sizeof(bigint));
	/*if(len!=0 && !isdigit(str[0]))
		return NULL;*/
	int len=strlen(str);
	if(len==0){
		return NULL;
	}
	if(len==1 && str[0]=='0')
	{
		p->a=(int*)malloc(2*sizeof(int));
		p->a[0]=0;
		p->size=1;
		return p;
	}
	
	if(!isdigit(str[0]))
	{
		p->a=(int*)malloc(sizeof(int));
		p->a[0]=0;
		p->size=0;
		return p;
	}
	p->a=( int *)malloc(sizeof(int)*len/5);
	int j=len/5;
	int n=0;
	int i=len/5;
	for(i=0;i<=len/5;i++){
		p->a[i]=0;
	}
	if(len%5==0) j-=1;
	int l=j;
	while(len>0){
		if(len>5){
			for(n=4;n>=0;n--){
				p->a[j]=p->a[j]*10+(str[len-n-1]-'0');
			}
		}
		else{
			for(n=0;n<len;n++){
				p->a[j]=p->a[j]*10+(str[n]-'0');
			}
		}
		len=len-5;
		j--;
	}
	p->size=l+1;
	return p;
}

void intal_destroy(void* intal){
	
		free((bigint *)intal);
}

char* intal2str(void* intal){
	bigint * p1=(bigint *)intal;
	if(intal==NULL)
	{
		char * str=(char*)malloc(sizeof(char)*4);
		strcpy(str,"NaN");
		return str;
	}

	/*if(p1->size==0 && p1->a[0]==0)
	{
		char * str=(char*)malloc(sizeof(char)*3);
		strcpy(str,"NaN");
		return str;
	}*/ 
		
	char * str=(char*)malloc(sizeof(char)*p1->size+1);
	int i;
	
	
	for(i=0;i<p1->size;i++){
		char *s=(char*)malloc(5);
		if(p1->a[i]>=0 && p1->a[i]<10 && i!=0){
			strcat(str,"0000");
		}
		if(p1->a[i]>=10 && p1->a[i]<100 && i!=0){
			strcat(str,"000");
		}
		if(p1->a[i]>=100 && p1->a[i]<1000 && i!=0){
			strcat(str,"00");
		}
		if(p1->a[i]>=1000 && p1->a[i]<10000 && i!=0){
			strcat(str,"0");
		}
		sprintf(s,"%d",(p1->a[i]));
		strcat(str,s);
	}
	return str;
}

void* intal_add(void* intal1, void* intal2)
{
bigint * p1=(bigint *)intal1;
	bigint * p2=(bigint *)intal2;
	if(intal1==NULL || intal2==NULL || (p1->a[0]==0) && (p1->size==0) || (p2->a[0]==0) && (p2->size=0)) 
		return NULL;
	int m;
	int n;
	if(p1->size>=p2->size)
	{
		m=p1->size+1;
		n=p2->size;
	}
	else
	{
		return intal_add(intal2,intal1);
	}
	bigint * res=(bigint *)malloc(sizeof(bigint));
	res->a=(int *)malloc(sizeof(int)*m);
	res->size=m;
	for(m=0;m<res->size;m++)
	{
		res->a[m]=0;
	}
	int size1=p1->size-1;
	int size2=p2->size-1;
	int flag=1;
	for(m=res->size-1;m>=res->size-n;m--)
	{
		res->a[m]=p1->a[size1--]+p2->a[size2--];
	}
	while(m>0)
	{
		res->a[m--]+=p1->a[size1--];
	}
	m=res->size-1;
	while(m){
		if(res->a[m]>99999)
		{
				int temp=res->a[m]-100000;
				int carry=res->a[m]/100000;
				res->a[m]=temp;
				res->a[--m]+=carry;
			}
			else
			{
				m=0;
			}
	}
	if(res->a[0]==0)
	{
		for(flag=1; flag<res->size;flag++)
		{
				res->a[flag-1]=res->a[flag];
		}
		res->a=realloc(res->a,sizeof(int)*(res->size-1));
		res->size--;
	}
	return res;

}

int intal_compare(void* intal1, void* intal2)
{

	bigint *p1=(bigint*)intal1;
	bigint* p2=(bigint*)intal2;
	
	if(!intal1 || !intal2)
		return -2;
	if (p1->size>p2->size)
		return 1;
	if(p2->size>p1->size)
		return -1;
	int i;
	if(!intal1||!intal2)
		return 0;
	for(i=0;i<p2->size;i++)
	{
		if(p1->a[i]>p2->a[i])
			return 1;
		else if(p1->a[i]<p2->a[i])
			return -1;
	}
	return 0;
}


void* intal_increment(void* intal)
{
	bigint *p=(bigint*)intal;
	if(!intal)
		return NULL;
	if(p->a[0]==0 && p->size==1)
	{
		return p;
	}
	char *str=(char*)malloc(sizeof(char));
	str[0]='1';
	
	bigint *t=(bigint*)intal_create(str);
	bigint *r=(bigint*)intal_add(p,t);

	return r;
}

void* intal_diff(void* intal1, void* intal2)
{
	if(intal1==NULL || intal2==NULL) 
		return NULL;

	bigint * p1=(bigint *)intal1;
	bigint * res=(bigint *)malloc(sizeof(bigint));
	bigint * p2=(bigint *)intal2;
	
	if(p1->a[0]==0) 
		return (void *)p2;
	else if(p2->a[0]==0)
	{ 
		return (void *)p1;
	}
	else
	{
		if(intal_compare(intal1,intal2)==0) 
		{
			res->a=(int *)malloc(sizeof(int));
			res->a[0]=0;
			res->size=1;
			return res;
		}
		else if(intal_compare(intal1,intal2)==-1)
		{
			return intal_diff(intal2,intal1);
		}
		else
		{
			int size1=p1->size-1;
			int size2=p2->size-1;
			res->a=(int *)malloc(sizeof(int)*size1);
			res->size=size1+1;
			int m;
			for(m=0;m<res->size;m++)
			{
				res->a[m]=p1->a[m];
			}
			int j=size1;
			while(size2>=0)
			{
				int diff=res->a[size1]-p2->a[size2];
				int flag=1;
				j=size1;
				if(diff>=0)
				{
					res->a[j]=diff;
					j--;
				}
				else
				{
					while(flag)
					{
						if(diff<0)
						{
							res->a[j]=(100000+diff);
							res->a[j-1]-=1;
							diff=res->a[j-1];
							flag=1;
							j--;
						}
						else
						{
							res->a[j]=diff;
							flag=0;
							
						}
					}
				}
					size1--;
					size2--;
			}
			while(j>0)
			{
				if(res->a[j]<0)
				{
					res->a[j]+=100000;
					res->a[j--]-=1;
				}
				else
				{
					break;
				}
			}
			if(res->a[0]==0 && res->size!=1)
			{
				int flag;
				for(flag=1; flag<res->size;flag++)
				{
					res->a[flag-1]=res->a[flag];
				}
				res->a=realloc(res->a,sizeof(int)*(res->size-1));
				res->size--;
			}
			return res;
		}
	}
}
void* intal_decrement(void* intal)
{
	
	if(!intal)
		return NULL;
	bigint *p=(bigint*)intal;
	
	if(p->a[0]==0 && p->size==1)
	{

		return p;
	}
	char *str=(char*)malloc(sizeof(char));
	str[0]='1';
	
	
	
		
	bigint *t=(bigint*)intal_create(str);
	bigint *r=(bigint*)intal_diff(p,t);
	return r;
	
}

void* intal_multiply(void* intal1, void* intal2)
{
	if(!intal1 || !intal2)
		return NULL;
	bigint *p1=(bigint*)intal1;
	bigint *p2=(bigint*)intal2;
	bigint *p=(bigint*)malloc(sizeof(bigint));
	
	int n1=p1->size;
	int n2=p2->size;
	int n=n1+n2;
	if(n1==0 || n2==0)
	{
		p->a=(int*)malloc(sizeof(int));
		p->a[0]=0;
		p->size=1;
		return p;
	}
	
	
		p->a=(int *)malloc(sizeof(int)*(p1->size+p2->size));
		int j=0;
		int total=(p1->size+p2->size);
		p->size=total;
		for(j=0;j<total;j++)
		{
			p->a[j]=0;
		}
		j-=1;
		int size1=p1->size-1;
		int size2=p2->size-1;
		j=p->size-1;
		while(size2>=0)
		{
			long int mul2=(long int)p2->a[size2--];
			size1=p1->size-1;
			int k=j;
			while(size1>=0)
			{
				long int mul1=(long int)p1->a[size1--];
				long int product=mul1*mul2;
				int l=product%((int)pow(10,5));
				product=product/pow(10,5);
				int h=product;
				p->a[k]+=l;
				int flag=1;
				int m=k;
				while(p->a[m]>99999)
				{
						int temp=p->a[m];
						p->a[m]=p->a[m]%(int)pow(10,5);
						p->a[--m]+=(temp)/pow(10,5);
				}
				flag=1;
				p->a[k-1]+=h;
				m=k-1;
				int count=0;
				while(p->a[m]>99999)
				{
						int temp=p->a[m];
						p->a[m]=temp%(int)pow(10,5);
						p->a[--m]+=(temp)/(int)pow(10,5);
				}
				k--;
			}
			j--;
		}
	int in=0;
		while(p->a[in]==0)
			in++;
		if(in){
			for(j=in;j<p->size;j++)
			{
				p->a[j-in]=p->a[j];
			}
			p->size-=in;
			p->a=realloc(p->a,sizeof(int)*(p->size));
		}
		return p;

}

void* intal_divide(void* intal1, void* intal2)
{
	if(intal1==NULL || intal2==NULL) return NULL;
	bigint * p1=(bigint *)intal1;
	bigint * p2=(bigint *)intal2;
	char *res=(char *)malloc(sizeof(bigint)*p1->size);
	int m=0;
	if(p2->a[0]==0)
	{
		return NULL;
	}
	if(p1->a[0]==0 || intal_compare(intal1,intal2)==-1)
	{
		return intal_create("0");
	}
	else if(intal_compare(intal1,intal2)==0)
	{
		return intal_create("1");
	}
	if(p2->a[0]==1 && p2->size==1)
	{
		return intal1;
	}
	else
	{
		int count=0;
		int size2=p2->size-1;
		int size1=p1->size-1;
		char *t1=intal2str(p1);
		char *t2=intal2str(p2);
		int len2=strlen(t2);
		int flag=1;
		if(strlen(t1)==strlen(t2))
		{
			while(intal_compare(p1,p2)!=-1)
			{
				p1=intal_diff(p1,p2);
				count+=1;
			}
			char *s=(char *)malloc(len2);
			sprintf(s,"%d",count);
			s=realloc(s,strlen(s));
			return intal_create(s);
		}
		else
		{
			int i=0; 
			int j=len2-1;
			char * dividend=(char*)malloc(len2+1);
			strncpy(dividend,t1,len2);
			int k=len2;
			if(intal_compare(intal_create(dividend),intal2)==-1)
			{
				if(strlen(t1)>len2)
				{
								strncat(dividend,&t1[len2],1);
								j++;
				k++;
				}
			}
			bigint * quotient =(bigint *)intal_create(dividend);
			count=0;
			flag=1;
			while(flag)
			{
				count=0;
				j++;
				while((intal_compare(quotient,p2)>=0))
				{
					count+=1;
					quotient=intal_diff(quotient,p2);
					flag=1;
				}
				sprintf(dividend,"%d",count);
				strcat(res,dividend);
				if(j>=strlen(t1))
				{
					flag=0;
				}
				else{
					flag=1;
					char * divi=intal2str(quotient);
					char *divi2=(char *)malloc(strlen(divi)+1);
					strcat(divi2,divi);
					strncat(divi2,&t1[j],1);
					quotient=intal_create(divi2);
					flag=1;
				}
			}
		}
		return intal_create(res);
	}

}

static void* pow_2(void *intal)
{
    return intal_multiply(intal,intal);
}

void* intal_pow(void* intal1, void* intal2)
{
	if(intal1==NULL || intal2==NULL) 
		return NULL;
	bigint * p1=(bigint *)intal1;
	bigint * p2=(bigint *)intal2;
	if(p1->a[0]==0 && p1->size==1) 
		return intal_create("0");
	if((p1->a[0]==1 && p1->size==1)|| (p2->a[0]==0 && p2->size==1)) 
		return intal_create("1");
	if(p2->a[0]==1 && p2->size==1) 
		{
			bigint *p=(bigint*)malloc(sizeof(bigint));
			p->a=(int*)malloc(p1->size*sizeof(int));
			for(int i=0;i<p1->size;i++)
				p->a[i]=p1->a[i];
			p->size=p1->size;
			return p;
		}
	char *s2=intal2str(intal2);
	int flag=1;
	if((s2[strlen(s2)-1]-'0')%2==0)
	{
		flag=0;
	}
	bigint *rem=(bigint*)malloc(sizeof(bigint));
	rem->a=(int *)malloc(sizeof(int));
	rem->size=1;
	rem->a[0]=flag;	
	return intal_multiply(pow_2(intal_pow(intal1,intal_divide(intal2,intal_create("2")))),intal_pow(intal1,rem));
}
/*char * Karatsuba(char *a,char *b,int k){
	int res;
	int t = k;
	if(k == 1){
		res =convert_to_num(a)*convert_to_num(b);
		return convert_to_arr(res);
	}
	if(is_even(k) != 0){
		k++;
	}
	int m2 = k/2;
	char *a1,*a2,*b1,*b2;
	a1 = allocate(m2);
	a2 = allocate(m2);
	b1 = allocate(m2);
	b2 = allocate(m2);
	split(a,m2,a1,a2);
	split(b,m2,b1,b2);
	char *p1,*p2,*p3,*a3,*b3;
	a3 = (char *)add_large_num(a1,a2);
	b3 = (char *)add_large_num(b1,b2);
	a3 = (char *)remove_zero(a3);
	b3 = (char *)remove_zero(b3);
	int a3len = length(a3);
	int b3len = length(b3);
	if(a3len < b3len){
		//a3 = make_equal(a3,b3len - a3len);
		b3 = (char *)remove_zero(b3);
		b3len = length(b3);
		if(a3len < b3len){
			a3 = make_equal(a3,b3len - a3len);
			a3len = length(a3);
			b3len = length(b3);
		}
		if(a3len > b3len){
			b3 = make_equal(b3,a3len - b3len);
			a3len = length(a3);
			b3len = length(b3);
		}
	}
	if(a3len > b3len){
		//b3 = make_equal(b3,a3len - b3len);
		a3 = (char *)remove_zero(a3);
		a3len = length(a3);
		if(a3len < b3len){
			a3 = make_equal(a3,b3len - a3len);
			a3len = length(a3);
			b3len = length(b3);
		}
		if(a3len > b3len){
			b3 = make_equal(b3,a3len - b3len);
			a3len = length(a3);
			b3len = length(b3);
		}
	}
	p1 = Karatsuba(a1,b1,m2);
	p2 = Karatsuba(a2,b2,m2);
	int x = a3len;
	p3 = Karatsuba(a3,b3,x);



	free_alloc(a1);
	free_alloc(a2);
	free_alloc(a3);
	free_alloc(b1);
	free_alloc(b2);
	free_alloc(b3);
	return get_result(p1,p2,p3,k);
}
*/