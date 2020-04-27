function [val,vec] = leverrier( A )
	[n, m] = size(A);
	if n ~= m
		error('Matricea trebuie sa fie patratica')
	end
	p = ones(1, n+1);
	I = eye(n);

	B = A;
	p(2) = -trace(B);
	for i = 2:n-1
		B = A * (B + p(i)*I);
		p(i+1) = -trace(B)/i;
	end
	p(n+1) = -trace(A * (B + p(n)*I))/n;
	
	val = roots(p);
	vec = ones(n, n);
	
	for i = 1:length(val)
		lambda = val(i);
		sys = A - lambda*I;
		[sol, ~] = linsolve(sys(:,2:end), sys(:,1));
		vec(2:end, i) = sol;
    end
end

