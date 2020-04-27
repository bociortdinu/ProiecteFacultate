A=rand(3);
[~,val_exacta]=eig(A);

val_exacta=diag(val_exacta);
[val, ~] = leverrier(A);
eroareleverrier=norm(val - val_exacta)