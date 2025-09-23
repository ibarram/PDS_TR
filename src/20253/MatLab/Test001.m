tic
a = floor(rdca(2));
a = a+1;
mxx= zeros(1,n_dia-1);
for i1=1:(n_dia-1)
    idi = (i1-1)*a+1;
    idf = i1*a;
    mxx(i1) = max(hi(idi:idf));
end
mx1 = max(mxx);
t1 = toc;

tic
x = hi(ones(6,1)*(1:13)+(0:5)'*13*ones(1,13));
mx2 = max(x,[],2);
t2 = toc;