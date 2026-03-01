import grpc
from concurrent import futures
import time
import math

import prime_pb2
import prime_pb2_grpc

class PrimeServicer(prime_pb2_grpc.PrimeServiceServicer):
    def IsPrime(self, request, context):
        n = request.number
        if n < 2:
            return prime_pb2.PrimeResponse(is_prime=False, factors=[n])
        factors = []
        temp = n
        d = 2
        while d * d <= temp:
            while temp % d == 0:
                factors.append(d)
                temp //= d
            d += 1 if d == 2 else 2 
        if temp > 1:
            factors.append(temp)
        is_prime = len(factors) == 1
        return prime_pb2.PrimeResponse(is_prime=is_prime, factors=factors)

    def FirstPrime(self, request, context):
        n = request.n
        primes = []
        candidate = 2
        while len(primes) < n:
            if self._is_prime(candidate):
                primes.append(candidate)
            candidate += 1
        return prime_pb2.FirstPrimeResponse(primes=primes)

    def IntervalPrime(self, request, context):
        min_val = request.min
        max_val = request.max
        for num in range(int(min_val), int(max_val) + 1):
            if self._is_prime(num):
                yield prime_pb2.PrimeNumber(value=num)

    def _is_prime(self, n):
        if n < 2:
            return False
        if n == 2:
            return True
        if n % 2 == 0:
            return False
        for i in range(3, int(math.sqrt(n)) + 1, 2):
            if n % i == 0:
                return False
        return True

def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    prime_pb2_grpc.add_PrimeServiceServicer_to_server(PrimeServicer(), server)
    server.add_insecure_port('[::]:1025')
    print("Serveur démarré sur le port 1025...")
    server.start()
    try:
        while True:
            time.sleep(86400) 
    except KeyboardInterrupt:
        server.stop(0)

if __name__ == '__main__':
    serve()
