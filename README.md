# SP-MekatronicProject
A school project about creating a Regulator

s=tf('s');
h=0.8
GR=2.282*(1+(1/(55.55*s)))
H=c2d(GR,h,'tustin')
format long
H.den{1}
H.num{1}
%2.2984320
%-2.2655679
