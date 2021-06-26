#include <iostream>
#include <string>

using namespace std;
class BigInteger
{
public:
    BigInteger(); // default constructor
    BigInteger(BigInteger& n); // copy constructor
    BigInteger(string str); // constructor with initialization using a specified string of digits
    bool isZero(); // check if the big integer is zero
    int size() { return length; } // return the length of the big integer
    BigInteger &operator +(BigInteger& n); // return the sum of this big integer object and a big integer n
    BigInteger &operator -(BigInteger& n); // return the difference by subtracting a big integer n from this big integer object
    BigInteger &operator *(int d); // return the multiplication of this big integer object and a single digit d (0<=d<=9)
    BigInteger &operator *(BigInteger& n); // return the multiplication of this big object and a big integer n
    BigInteger &operator /(BigInteger& n); // return the quotient of dividing this big integer object by a big integer n
    BigInteger &operator =(const BigInteger& n);
    bool operator >(BigInteger& n); // check if this big integer object is larger than the big integer n
    bool operator <(BigInteger& n); // check if this big integer object is smaller than the big integer n
    bool operator ==(BigInteger& n); // check if this big integer object is equal to the big integer n
    bool operator >=(BigInteger& n); // check if this big integer object is larger than or equal to the big integer n

    friend BigInteger &Abs(const BigInteger& n); // a friend function returning the absolute value of the big number n
    friend ostream& operator<<(ostream& output, BigInteger& n); // overloading the insertion operator for outputting a big number

private:
    int length; // length of the big integer
    int* digits; // an array storing the digits of the big integer
    int sign; // the sign of the big integer
};

BigInteger::BigInteger() {
    digits = new int[1000];
    length = 1;
    sign = 0;
    digits[0] = 0;



};

BigInteger::BigInteger(BigInteger& n)
{
    digits = new int[1000];
    length = n.length;
    sign = n.sign;
    for (int i=0; i<n.length; i++){//copy
        digits[i]=n.digits[i];
    }
}

BigInteger::BigInteger(string str)
{
    digits = new int[1000];
    if (str[0]=='-')//if it is negative
    {
        sign = -1;
        length = str.length()-1;//remove the negative sign
        for (int i=1; i<=length; i++){
            digits[i - 1]=(str[i]-'0');
        }
    }
    else
    {
        sign =1;
        length = str.length();
        for (int i=0; i<length; i++){
            digits[i]= (str[i]-'0');
        }
    }
}

bool BigInteger::isZero()
{
    if(length==0){//check if it is 0
        return 1;
    }
    else{
        return 0;
    }
}

BigInteger &BigInteger:: operator =(const BigInteger& n)
{
    sign = 0;
    length = 0;
    digits = new int[100];
    for(int i = 0; i < 100; i++){//check if it is all the same
        digits[i] = n.digits[i];
    }
    sign = n.sign;
    length = n.length;
    return *this;
}

BigInteger &BigInteger::operator +(BigInteger& n)
{
    BigInteger *add =new BigInteger();
    string str = "";
    int big, small;
    int sum, carry;
    int count =-1;
    if (sign==n.sign){//mark
        count=1;
    }
    else{
        count=0;
    }
    if(count==1){//sign same
        big = length - 1;
        small = n.length - 1;
        sum = 0,carry=0;
        while(big >= 0 || small >= 0){//not till the end
            sum = carry;
            if(big >= 0){
                sum = sum +digits[big];
            }
            if(small >= 0){
                sum = sum + n.digits[small];
            }
            carry = sum / 10;//carry = except the digits
            sum %= 10;//only the digits
            str = ((char)(sum + '0')) + str;//put the digits in
            big--;//then do the next number so big-- small--
            small--;
        }
        if(sign == -1){//if it is negative number + negative number
            str = "-" + str;
        }
        add = new BigInteger(str);
    }
    else if(count==0){// if it is +number1 + (-number2) == number1 - number2
        BigInteger temp;
        BigInteger temp2;
        int backk=0;
        if(Abs(*this) > Abs(n)){
            count=1;
        }
        else if(Abs(*this) < Abs(n)){
            count=0;
        }
        if(count==1){//big-small

            temp = Abs(*this);
            temp2 = Abs(n);
        }
        else if (count==0){//do the opposite
            temp2 = Abs(*this);
            temp = Abs(n);
        }
        //cout << *this << n << endl;
        big = temp.length - 1;//starting from the units digits
        small = temp2.length - 1;
        //cout << big << small << endl;
        while(big >= 0 || small >= 0){//not till end
            sum = temp.digits[big];
            if(temp2.digits[small] > temp.digits[big] && small >= 0){//big units digits < small units digits
                backk = big - 1;//find carry
                //cout << "x" << endl;
                while(temp.digits[backk] == 0){//find digits until which is not 0
                    backk--;
                    //cout << endl << "back" << backk << endl;

                }
                temp.digits[backk]=temp.digits[backk]-1;//carry number has to -1
                //cout << temp.digits[backk] << endl;
                if (backk+1==big -1){//if it just carry one
                    temp.digits[backk+1] = temp.digits[backk+1]+9;
                    //cout << "this is i " << backk+1 << "this is temp+9   " << temp.digits[backk+1];
                }
                else{
                    for(int i = backk + 1; i < big ; i++){//add 9 to the digits which is 0
                    temp.digits[i] = temp.digits[i]+9;
                    //cout << "this is i " << i << "  this is temp+9   " << temp.digits[i] << endl;
                }

                }
                sum =10 - temp2.digits[small] + sum;//compute units digits
            }
            else{
                if(small >= 0){
                    sum =sum - temp2.digits[small];//not till the end and the big units digits is large then the small units digits
                }
            }
            str = ((char)(sum + '0')) + str;
            big--;
            small--;
            //cout << "big: " << big << "  small: " << small << endl;
            //cout << temp2.digits[small] << "  zzz  " <<  temp.digits[big] << endl;
        }
        /*for (int i=0; i<str.length(); i++){
            cout << str[i] << endl;
        }*/
        //cout << str[0] << endl;
        if (str[0]=='0'){//if the first digits is 0 then erase
            str.erase(str.begin());
        }
        if((count==1 && sign == -1)||(count==0 && n.sign == -1)){// if it is negative
            str = "-" + str;
        }
        add = new BigInteger(str);
    }
    return *add;
}

 BigInteger &BigInteger::operator -(BigInteger& n)
 {
        /*BigInteger temp(n);
        string a(length+1,'0');
        string b(length+1,'0');
        for(int i=0; i<length; i++){
            temp.digits[i]=digits[i];
        }
        BigInteger temp2(a);
        temp.sign = -1;
        return (temp + temp2);*/
        /*if (n.sign==1){
            n.sign=-1;
            cout << n.sign << endl;
            return (*this + n);
            n.sign=1;
            cout << "nest" << n.sign << endl;
        }

        if (n.sign==-1){
            n.sign=1;
            cout << n.sign << endl;
            return(*this + n);
            n.sign=-1;
            cout << "nest" << n.sign << endl;
        }*/
        BigInteger temp1(*this);//put into temp and temp1 to make sure it doesnt affect the original value
        BigInteger temp(n);
        temp.sign *= -1;//since it is -, so we change the sign and put into + to compute
        return (temp1 + temp);
 }


BigInteger &BigInteger:: operator *(int d)
{
    //cout << "d: " << d << endl;
    BigInteger *mult = new BigInteger(*this);
    for (int i=0; i<mult->length; i++){
        mult->digits[i] = mult->digits[i]*d;//multiply all the digits by d
    }

    for (int i=mult ->length-1; i>0; i--){
        if (mult->digits[i]>=10){//if the digits multiply by d is larger then  large then 10 we need to carry out
            mult->digits[i-1] = mult->digits[i-1] + mult->digits[i]/10;//carry the number except units digits
            mult->digits[i] = mult->digits[i]%10;
        }
    }
    if (mult->digits[0]>=10){// if the bigest digits is lager then 10, we have to create more length to store the carry out
        mult->length =mult->length+1 ;
        for (int i =mult->length; i>0; i--){// move all the digits back so the we have we can add in the first one
        mult->digits[i] = mult->digits[i-1];
    }
    mult-> digits[0] = mult-> digits[1]/10;//onle save the unites digits
    mult-> digits[1] = mult-> digits[1]%10;// carry out
    }
    return *mult;


}


BigInteger&BigInteger ::operator *(BigInteger& n)
{
    BigInteger *mult = new BigInteger();
    for (int i = 0; i < n.length; i++) {
        *mult = (*mult * 10) + ((*this) * n.digits[i]);/*example 123
    }                                                           x 11
                                                              ------
                                                                 123
                                                                1230 -> multiplt by 10 and add 123
    */
    return *mult;


}
}

BigInteger&BigInteger ::operator /(BigInteger& n)
{
    BigInteger temp;
    BigInteger temp2;
    temp = Abs(*this);
    temp2 = Abs(n);//store
    string str="";
    int t;
    BigInteger num2;
    BigInteger tnum;
    num2 = Abs(n);
    int count=0;
    tnum = temp;//store this
    t = tnum.length-temp2.length;
    while(tnum>=temp2){//tnum >= n so it can be devide
        //cout << tnum << "--------" << temp2 <<endl;
        //cout << tnum.length << "----" <<temp2.length << endl;;
        num2 = Abs(n);
        for (int j=0; j<t; j++){
            num2  = num2*10;
        }/*
          example
          2800 / 12 = 2800 - 1200(1) -1200(2) = 400 <= 1200 so we add 2 into the string
          2100 = 21*10*10 so this is why we multiply by 10 until its length is same as this
          then 400 / 120 = 400 - 120(1) -120(2) - 120(3) = 40 <= 120 so we add 3 in to the string
          then 40 / 12 = 40 - 12(1) - 12(2) -12(3) =4 so we add 3 into string
          and then we can get the answer = 233
          */
        t--;
        //cout << tnum << " xxx " << num2 << endl;
        count=0;
        BigInteger temp;
        while(tnum>=num2){
            //cout << tnum << "----" << num2 << "    " << endl;;
            tnum = tnum-num2;
            //cout << tnum << endl;
            count++;// to see how many times it can be - until it is not 0
        }
        //cout << "this is count" << count << endl;
        str = ((char)(count+'0'))+ str;
        }
        string b="";
    if(sign==-1||n.sign==-1){//add - if it is negative
        b = "-" + b;
    }
    for (unsigned int  i=0; i<str.length(); i++){//cause it start from the begin so need to reverse
        b =  b + str[str.length()-1-i];
    }
    BigInteger *ans;
    ans = new BigInteger(b);
    return *ans;
    }

bool BigInteger::operator >(BigInteger& n)
{
    if (length > n.length && (sign ==1 && n.sign==1))//n1 longer than n2 and are all +
    {
        return 1;
    }
    else if (sign==1 && n.sign ==-1)//n1 + n2 -
    {
        //cout << "this" << endl;
        return 1;
    }
    else if (sign==-1 && n.sign ==1)//n1 - n2 +
    {
        return 0;
    }
    else if (length > n.length && (sign==-1 && n.sign ==-1))//n1 longer than n2 and are all -
    {
        return 0;
    }
    else if (length < n.length && (sign==1 && n.sign==1))//n2 longer than n1 and are all +
    {
        return 0;
    }
    else if (length < n.length && (sign==-1 && n.sign ==-1))//n2 longer than n1 and are all -
    {
        return 1;
    }
    else if (length == n.length && (sign==1 && n.sign==1))// same than compare
    {
        for (int i=0; i<length; i++)
        {
            if (digits[i] > n.digits[i])
            {
                return 1;
                break;
            }
            else if(digits[i] < n.digits[i])
            {
                return 0;
                break;
            }
        }
    }
    return false;
}

bool BigInteger::operator <(BigInteger& n)
{
    if (length < n.length && (sign==1 && n.sign==1)){//n2 longer then n2 and is all postive
        return 1;
    }
    else if (length > n.length && (sign ==-1 && n.sign==-1)){//n1 longer then n2 and is all negative
        return 1;
    }
    else if (sign==-1 && n.sign ==1){//n2 is positive and n2 is negavite
        return 1;
    }
    else if (length == n.length){// if they are same long
        if (sign==1 && n.sign ==1){//see when they are all positive which one is large
            for (int i=0; i<=length; i++){
                if (digits[i]<n.digits[i]){
                    return 1;
                    break;
                }
            }
            return 0;
        }
    }
    return 0;
}

bool BigInteger::operator ==(BigInteger& n)
{
    /*for (int i=length; i>=0; i--){
    cout << digits[i] << endl;
    }
    for (int i=length; i>=0; i--){
        cout << n.digits[i] << endl;
    }*/
    int count;
    //cout << sign << endl <<n.sign << endl;
    if (length==n.length && ((sign==1 && n.sign==1) || (sign==-1 && n.sign==-1))){//is they are both positive or negative and the length is the same
            for (int i=length; i>=0; i--){
            //cout << "---debug---" << "1: " << digits[i] <<" 2: " << n.digits[i] << endl;
            if (digits[i]==n.digits[i]){//all the digits are the same
                count=1;
            }
            else {
                count=0;
                break;
            }
        }
    }
    else{
        return 0;
    }
    if (count==1){
        return 1;
    }
    else{
        return 0;
    }

}

bool BigInteger::operator >=(BigInteger& n)
{
    int count=0;
    if (length > n.length && (sign == 1 && n.sign ==1)){//n1 longer then n2 and are all positive
    return 1;
    }
    else if (sign==1 && n.sign ==-1){//n1 is positive and n2 is negative
         return 1;
    }
    else if (length < n.length && (sign==-1 && n.sign ==-1)){// both are negative and n2 longer then n1
        return 1;
    }
    else if(length==n.length && ((sign==1 && n.sign==1) || (sign==-1 && n.sign==-1))){//if they are same long and are all positive or negative
            for (int i=0; i<length; i++){
            if (digits[i]==n.digits[i]){//all digits are the same
            //cout << digits[i] << "  " << n.digits[i] <<endl;
            count=1;
            }
            else{
            if(digits[i]>n.digits[i]){//check if n2 is large
                return 1;
                count=0;
            }
            else{
                count=0;
                break;
            }
            }
        }
    }
    if (count ==1){
        return 1;
    }
    else {
        return 0;
    }

}

BigInteger &Abs(const BigInteger& n)
{
    BigInteger *temp = new BigInteger();
    *temp = n;
    temp -> sign = 1;//change to positive
    //cout << "abs : " << *temp << endl;
    return *temp;
}

ostream& operator<<(ostream& output, BigInteger& n)
{
    int count = 0;
    for(int i = 0; i < n.length; i++){//output all digits
        if(n.digits[i] == 0){
            count++;
        }
    }


    if(count == n.length){//if all digits are 0 then just output 0
        output << 0;
    }
    else if(n.sign == -1){//if it is negative then outout - first then output the digits
        output << '-';
        for(int i = 0; i < n.length; i++){
            output << n.digits[i];
        }
    }
    else{
        for(int i = 0; i < n.length; i++){//not negative just output all the digits
            output << n.digits[i];
        }
    }
    return output;
}
int main()
{
    BigInteger n1("-9999888852245546546365637657476365463665465"), n2("3045452545446132155444422321");
    BigInteger n3("-123456789987654321"), n4("-123456789987654321");
    BigInteger n, t1, t2, t3, t4, t5, t6, t7;
    cout << n2.size() << endl;
    cout << (t2 == n1) << endl;
    t1 = n1 / n2;
    t2 = n1 - n2;
    t3 = n1 + n2;
    t4 = n1 * n2;
    t5 = n1 * 4;
    t6 = n3 + n4;
    t7 = n3 - n4;
    cout << t1 << endl;
    cout << t2 << endl;
    cout << t3 << endl;
    cout << t4 << endl;
    cout << t5 << endl;
    cout << t6 << endl;
    cout << t7 << endl;
    cout << (n3 >= n4) << endl;
    cout << (n3 == n4) << endl;
    cout << (n3 > n1) << endl;
    cout << (n3 < n2) << endl;
}
