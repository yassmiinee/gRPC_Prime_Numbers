import grpc
import prime_pb2
import prime_pb2_grpc

def run():
    channel = grpc.insecure_channel('localhost:50051')
    stub = prime_pb2_grpc.PrimeServiceStub(channel)

    print("=== Test IsPrime ===")
    for num in [17, 100, 2, 97, 1]:
        response = stub.IsPrime(prime_pb2.PrimeRequest(number=num))
        if response.is_prime:
            print(f"{num} est premier.")
        else:
            facteurs = list(response.factors)
            print(f"{num} n'est pas premier. Facteurs : {facteurs}")

    print("\n=== Test FirstPrime ===")
    for n in [5, 10]:
        response = stub.FirstPrime(prime_pb2.FirstPrimeRequest(n=n))
        print(f"Les {n} premiers nombres premiers : {list(response.primes)}")

    print("\n=== Test IntervalPrime (streaming) ===")
    intervals = [(10, 20), (30, 40)]
    for min_val, max_val in intervals:
        request = prime_pb2.IntervalRequest(min=min_val, max=max_val)
        print(f"Nombres premiers entre {min_val} et {max_val} : ", end="")
        try:
            for prime in stub.IntervalPrime(request):
                print(prime.value, end=" ")
        except grpc.RpcError as e:
            print(f"Erreur gRPC : {e.code()} - {e.details()}")
        print()

if __name__ == '__main__':
    run()