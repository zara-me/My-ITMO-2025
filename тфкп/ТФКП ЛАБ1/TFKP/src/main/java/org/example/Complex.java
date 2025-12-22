package org.example;

class Complex {
    double re;
    double im;

    public Complex(double re, double im) {
        this.re = re;
        this.im = im;
    }

    public Complex add(Complex other) {
        return new Complex(this.re + other.re, this.im + other.im);
    }

    public Complex subtract(Complex other) {
        return new Complex(this.re - other.re, this.im - other.im);
    }

    public Complex multiply(double k) {
        return new Complex(this.re * k, this.im * k);
    }

    public Complex multiply(Complex other) {
        return new Complex(this.re * other.re - this.im * other.im,
                this.re * other.im + this.im * other.re);
    }

    public Complex divide(Complex other) {
        double denom = other.re * other.re + other.im * other.im;
        return new Complex((this.re * other.re + this.im * other.im) / denom,
                (this.im * other.re - this.re * other.im) / denom);
    }

    public Complex pow(int n) {
        Complex res = new Complex(this.re, this.im);
        Complex ans = new Complex(1, 0);
        for (int i = 0; i < n; i++) ans = ans.multiply(res);
        return ans;
    }

    public double abs() {
        return Math.hypot(re, im);
    }

    @Override
    public String toString() {
        return re + " + " + im + "i";
    }
}
