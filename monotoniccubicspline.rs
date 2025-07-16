fn main() {
    // Read number of points
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let n: usize = input.trim().parse().unwrap();

    // Read x values
    let mut x: Vec<f64> = Vec::with_capacity(n);
    for _ in 0..n {
        input.clear();
        std::io::stdin().read_line(&mut input).unwrap();
        x.push(input.trim().parse().unwrap());
    }

    // Calculate deltas (differences between consecutive points)
    let mut delta: Vec<f64> = Vec::with_capacity(n - 1);
    for i in 0..(n - 1) {
        delta.push(x[i + 1] - x[i]);
    }

    // Initialize slope array (m)
    let mut m: Vec<f64> = vec![0.0; n];
    
    // Calculate initial slopes for interior points
    for i in 1..(n - 1) {
        if delta[i - 1] * delta[i] < 0.0 {
            m[i] = 0.0;
        } else {
            m[i] = (delta[i - 1] + delta[i]) / 2.0;
        }
    }

    // Set slopes for endpoints
    m[0] = delta[0];
    m[n - 1] = delta[n - 2];

    // Handle zero deltas
    for i in 0..(n - 1) {
        if delta[i] == 0.0 {
            m[i] = 0.0;
            m[i + 1] = 0.0;
        }
    }

    // Adjust slopes to ensure monotonicity
    for i in 0..(n - 1) {
        if delta[i] != 0.0 {
            let a = m[i] / delta[i];
            let b = m[i + 1] / delta[i];
            let h = (a * a + b * b).sqrt();
            
            if a * a + b * b > 9.0 {
                println!("a"); // Debug output as in original
                let tau = 3.0 / h;
                m[i] = tau * a * delta[i];
                m[i + 1] = tau * b * delta[i];
            }
        }
    }

    // Interpolate and print results
    for i in 0..=10 * (n - 1) {
        let t = i as f64 / 10.0;
        let ind = t.floor() as usize;
        let n = t - ind as f64;

        // Hermite basis functions
        let h00 = (1.0 + 2.0 * n) * (1.0 - n) * (1.0 - n);
        let h10 = n * (1.0 - n) * (1.0 - n);
        let h01 = n * n * (3.0 - 2.0 * n);
        let h11 = n * n * (n - 1.0);

        // Calculate interpolated value
        let out = x[ind] * h00 + m[ind] * h10 + x[ind + 1] * h01 + m[ind + 1] * h11;
        println!("{}", out);
    }
}
