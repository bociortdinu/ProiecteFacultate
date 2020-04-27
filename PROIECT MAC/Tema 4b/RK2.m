function [x, y] = RK2(f, a, b, y0, n)
    format long;
    h = (b-a)/n;
    if h > 0.001
        h = 0.001;
    end
    x = a:h:(a+h*n);
    lungimeInterval = length(x) - 1;
    if(size(f, 1) == 1 && size(f, 2) > 0)
        y = zeros(size(f, 2), length(x));
    else
        error('Ecuatia (sistemul de ecuatii) trebuie dat ca un element (matrice coloana)!');
    end
   
    if size(f, 2) == 1
        f = {f};
    end
    for j = 1:size(f, 2)
        y(j, 1) = y0(j);
    end
    coef = [0, 1];
    k = zeros(length(coef) + 1);
    if nargin(f{1}) == 2
        for i = 1:lungimeInterval
            
            for j = 1:length(coef)
                k(j+1) = f{1}(x(i) + h * coef(j), y(1, i) + h * coef(j) * k(j));
            end
            j = 2;
            y(1, i+1) = y(1, i) + h/2 * (k(j) + k(j+1));
        end
    end
    if nargin(f{1}) == 3        
        l = k;
        for i = 1:lungimeInterval
            
            for j = 1:length(coef)
                k(j+1) = f{1}(x(i) + h * coef(j), y(1, i) + h * coef(j) * k(j), y(2, i) + h * coef(j) * l(j));
                l(j+1) = f{2}(x(i) + h * coef(j), y(1, i) + h * coef(j) * k(j), y(2, i) + h * coef(j) * l(j));
            end
            j = 2;
            y(1, i+1) = y(1, i) + h/2 * (k(j) + k(j+1));
            y(2, i+1) = y(2, i) + h/2 * (l(j) + l(j+1));
        end
    end
end

