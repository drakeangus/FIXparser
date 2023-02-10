You'll need a FIX dictionary xml to create the mapping

run the `generate.sh` script with the path to the fix dictionary as an argument eg. `./generate.sh CustomFIX44.xml`

This will generate a header file with the custom mapping and compile `fix_tags_for_noobs.cpp` into an executable `fix_tags_for_noobs`


`fix_tags_for_noobs` takes stdin and outputs a csv output 
eg.

```
echo '40=2' | fix_tags_for_noobs 
Output:
40,(OrdType),=,2 (LIMIT)
```

This means you can use an alias like 
```
parse() { echo "$@" | fix_tags_for_noobs | column -t -s ',';}
```
To easily queery tags from the command line e.g.
```
$ parse 59
59  (TimeInForce)  =  

$ parse 59=1
59  (TimeInForce)  =  1 (GOOD_TILL_CANCEL)

$ parse '59=1|40=1'
59  (TimeInForce)  =  1 (GOOD_TILL_CANCEL)
40  (OrdType)      =  1 (MARKET)
```
Or whole fix message:
```
parse '8=FIX.4.4|9=418|35=D|34=3|49=SEB|52=20230208-18:24:40.160|56=FLEX|122=20230208-10:48:17.982|43=Y|11=TestOrder-20230208-12:47:48-0|21=3|100=XNGS|55=GOOG|65=US|48=BYY88Y7|22=2|461=ESXXXX|167=CS|207=XNGS|106=Alphabet|107=Alphabet Class C|54=2|60=20230208-12:47:48|38=3000|40=1|15=USD|59=1|20123=2193810997.44864|20100=2163418492|10048=US02079K1079|20121=AD|20110=20230113|10049=GOOG.O|20013=549300DNI5UYVM6X6N69|78=1|79=STIFTUTL|80=3000|10=001|'
8      (BeginString)                                  =  FIX.4.4
9      (BodyLength)                                   =  418
35     (MsgType)                                      =  D (ORDER_SINGLE)
34     (MsgSeqNum)                                    =  3
49     (SenderCompID)                                 =  Sender
52     (SendingTime)                                  =  30230101-12:00:00.000
56     (TargetCompID)                                 =  Target
122    (OrigSendingTime)                              =  30230101-12:00:00.000
43     (PossDupFlag)                                  =  Y
11     (ClOrdID)                                      =  123456
21     (HandlInst)                                    =  3 (MANUAL_ORDER)
100    (ExDestination)                                =  XNGS
55     (Symbol)                                       =  GOOG
65     (SymbolSfx)                                    =  US
48     (SecurityID)                                   =  BYY88Y7
22     (SecurityIDSource)                             =  2 (SEDOL)
461    (CFICode)                                      =  ESXXXX
167    (SecurityType)                                 =  CS (COMMON_STOCK)
207    (SecurityExchange)                             =  XNGS
106    (Issuer)                                       =  Alphabet
107    (SecurityDesc)                                 =  Alphabet Class C
54     (Side)                                         =  2 (SELL)
60     (TransactTime)                                 =  30230208-12:00:00
38     (OrderQty)                                     =  3000
40     (OrdType)                                      =  1 (MARKET)
15     (Currency)                                     =  USD
59     (TimeInForce)                                  =  1 (GOOD_TILL_CANCEL)
78     (NoAllocs)                                     =  1
79     (AllocAccount)                                 =  Acc
80     (AllocQty)                                     =  3000
10     (CheckSum)                                     =  001
```

More sophisticated use cases include:
```
parse() { [[ -p /dev/stdin ]] && input=$(cat -) || input="$@"; [[ -z $input ]] && return 1; echo $input | fix_tags_for_noobs | column -t -s ',' ;}
```
Which lets the parse function take arguemnts OR standin
```
$ echo '167=CS|35=F' | parse
167  (SecurityType)  =  CS (COMMON_STOCK)
35   (MsgType)       =  F (ORDER_CANCEL_REQUEST)
```
