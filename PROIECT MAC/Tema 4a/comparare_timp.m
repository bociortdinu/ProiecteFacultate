A=rand(3);
tic;
[val,vec]=leverrier(A);
toc
tic;
[vec,val]=eig(A);
toc
