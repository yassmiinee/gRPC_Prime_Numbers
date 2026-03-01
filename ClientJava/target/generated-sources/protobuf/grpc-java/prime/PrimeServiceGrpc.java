package prime;

import static io.grpc.MethodDescriptor.generateFullMethodName;

/**
 */
@javax.annotation.Generated(
    value = "by gRPC proto compiler (version 1.62.2)",
    comments = "Source: prime.proto")
@io.grpc.stub.annotations.GrpcGenerated
public final class PrimeServiceGrpc {

  private PrimeServiceGrpc() {}

  public static final java.lang.String SERVICE_NAME = "prime.PrimeService";

  // Static method descriptors that strictly reflect the proto.
  private static volatile io.grpc.MethodDescriptor<prime.Prime.PrimeRequest,
      prime.Prime.PrimeResponse> getIsPrimeMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "IsPrime",
      requestType = prime.Prime.PrimeRequest.class,
      responseType = prime.Prime.PrimeResponse.class,
      methodType = io.grpc.MethodDescriptor.MethodType.UNARY)
  public static io.grpc.MethodDescriptor<prime.Prime.PrimeRequest,
      prime.Prime.PrimeResponse> getIsPrimeMethod() {
    io.grpc.MethodDescriptor<prime.Prime.PrimeRequest, prime.Prime.PrimeResponse> getIsPrimeMethod;
    if ((getIsPrimeMethod = PrimeServiceGrpc.getIsPrimeMethod) == null) {
      synchronized (PrimeServiceGrpc.class) {
        if ((getIsPrimeMethod = PrimeServiceGrpc.getIsPrimeMethod) == null) {
          PrimeServiceGrpc.getIsPrimeMethod = getIsPrimeMethod =
              io.grpc.MethodDescriptor.<prime.Prime.PrimeRequest, prime.Prime.PrimeResponse>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.UNARY)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "IsPrime"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  prime.Prime.PrimeRequest.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  prime.Prime.PrimeResponse.getDefaultInstance()))
              .setSchemaDescriptor(new PrimeServiceMethodDescriptorSupplier("IsPrime"))
              .build();
        }
      }
    }
    return getIsPrimeMethod;
  }

  private static volatile io.grpc.MethodDescriptor<prime.Prime.FirstPrimeRequest,
      prime.Prime.FirstPrimeResponse> getFirstPrimeMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "FirstPrime",
      requestType = prime.Prime.FirstPrimeRequest.class,
      responseType = prime.Prime.FirstPrimeResponse.class,
      methodType = io.grpc.MethodDescriptor.MethodType.UNARY)
  public static io.grpc.MethodDescriptor<prime.Prime.FirstPrimeRequest,
      prime.Prime.FirstPrimeResponse> getFirstPrimeMethod() {
    io.grpc.MethodDescriptor<prime.Prime.FirstPrimeRequest, prime.Prime.FirstPrimeResponse> getFirstPrimeMethod;
    if ((getFirstPrimeMethod = PrimeServiceGrpc.getFirstPrimeMethod) == null) {
      synchronized (PrimeServiceGrpc.class) {
        if ((getFirstPrimeMethod = PrimeServiceGrpc.getFirstPrimeMethod) == null) {
          PrimeServiceGrpc.getFirstPrimeMethod = getFirstPrimeMethod =
              io.grpc.MethodDescriptor.<prime.Prime.FirstPrimeRequest, prime.Prime.FirstPrimeResponse>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.UNARY)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "FirstPrime"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  prime.Prime.FirstPrimeRequest.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  prime.Prime.FirstPrimeResponse.getDefaultInstance()))
              .setSchemaDescriptor(new PrimeServiceMethodDescriptorSupplier("FirstPrime"))
              .build();
        }
      }
    }
    return getFirstPrimeMethod;
  }

  private static volatile io.grpc.MethodDescriptor<prime.Prime.IntervalRequest,
      prime.Prime.PrimeNumber> getIntervalPrimeMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "IntervalPrime",
      requestType = prime.Prime.IntervalRequest.class,
      responseType = prime.Prime.PrimeNumber.class,
      methodType = io.grpc.MethodDescriptor.MethodType.SERVER_STREAMING)
  public static io.grpc.MethodDescriptor<prime.Prime.IntervalRequest,
      prime.Prime.PrimeNumber> getIntervalPrimeMethod() {
    io.grpc.MethodDescriptor<prime.Prime.IntervalRequest, prime.Prime.PrimeNumber> getIntervalPrimeMethod;
    if ((getIntervalPrimeMethod = PrimeServiceGrpc.getIntervalPrimeMethod) == null) {
      synchronized (PrimeServiceGrpc.class) {
        if ((getIntervalPrimeMethod = PrimeServiceGrpc.getIntervalPrimeMethod) == null) {
          PrimeServiceGrpc.getIntervalPrimeMethod = getIntervalPrimeMethod =
              io.grpc.MethodDescriptor.<prime.Prime.IntervalRequest, prime.Prime.PrimeNumber>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.SERVER_STREAMING)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "IntervalPrime"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  prime.Prime.IntervalRequest.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  prime.Prime.PrimeNumber.getDefaultInstance()))
              .setSchemaDescriptor(new PrimeServiceMethodDescriptorSupplier("IntervalPrime"))
              .build();
        }
      }
    }
    return getIntervalPrimeMethod;
  }

  /**
   * Creates a new async stub that supports all call types for the service
   */
  public static PrimeServiceStub newStub(io.grpc.Channel channel) {
    io.grpc.stub.AbstractStub.StubFactory<PrimeServiceStub> factory =
      new io.grpc.stub.AbstractStub.StubFactory<PrimeServiceStub>() {
        @java.lang.Override
        public PrimeServiceStub newStub(io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
          return new PrimeServiceStub(channel, callOptions);
        }
      };
    return PrimeServiceStub.newStub(factory, channel);
  }

  /**
   * Creates a new blocking-style stub that supports unary and streaming output calls on the service
   */
  public static PrimeServiceBlockingStub newBlockingStub(
      io.grpc.Channel channel) {
    io.grpc.stub.AbstractStub.StubFactory<PrimeServiceBlockingStub> factory =
      new io.grpc.stub.AbstractStub.StubFactory<PrimeServiceBlockingStub>() {
        @java.lang.Override
        public PrimeServiceBlockingStub newStub(io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
          return new PrimeServiceBlockingStub(channel, callOptions);
        }
      };
    return PrimeServiceBlockingStub.newStub(factory, channel);
  }

  /**
   * Creates a new ListenableFuture-style stub that supports unary calls on the service
   */
  public static PrimeServiceFutureStub newFutureStub(
      io.grpc.Channel channel) {
    io.grpc.stub.AbstractStub.StubFactory<PrimeServiceFutureStub> factory =
      new io.grpc.stub.AbstractStub.StubFactory<PrimeServiceFutureStub>() {
        @java.lang.Override
        public PrimeServiceFutureStub newStub(io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
          return new PrimeServiceFutureStub(channel, callOptions);
        }
      };
    return PrimeServiceFutureStub.newStub(factory, channel);
  }

  /**
   */
  public interface AsyncService {

    /**
     */
    default void isPrime(prime.Prime.PrimeRequest request,
        io.grpc.stub.StreamObserver<prime.Prime.PrimeResponse> responseObserver) {
      io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall(getIsPrimeMethod(), responseObserver);
    }

    /**
     */
    default void firstPrime(prime.Prime.FirstPrimeRequest request,
        io.grpc.stub.StreamObserver<prime.Prime.FirstPrimeResponse> responseObserver) {
      io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall(getFirstPrimeMethod(), responseObserver);
    }

    /**
     */
    default void intervalPrime(prime.Prime.IntervalRequest request,
        io.grpc.stub.StreamObserver<prime.Prime.PrimeNumber> responseObserver) {
      io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall(getIntervalPrimeMethod(), responseObserver);
    }
  }

  /**
   * Base class for the server implementation of the service PrimeService.
   */
  public static abstract class PrimeServiceImplBase
      implements io.grpc.BindableService, AsyncService {

    @java.lang.Override public final io.grpc.ServerServiceDefinition bindService() {
      return PrimeServiceGrpc.bindService(this);
    }
  }

  /**
   * A stub to allow clients to do asynchronous rpc calls to service PrimeService.
   */
  public static final class PrimeServiceStub
      extends io.grpc.stub.AbstractAsyncStub<PrimeServiceStub> {
    private PrimeServiceStub(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected PrimeServiceStub build(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      return new PrimeServiceStub(channel, callOptions);
    }

    /**
     */
    public void isPrime(prime.Prime.PrimeRequest request,
        io.grpc.stub.StreamObserver<prime.Prime.PrimeResponse> responseObserver) {
      io.grpc.stub.ClientCalls.asyncUnaryCall(
          getChannel().newCall(getIsPrimeMethod(), getCallOptions()), request, responseObserver);
    }

    /**
     */
    public void firstPrime(prime.Prime.FirstPrimeRequest request,
        io.grpc.stub.StreamObserver<prime.Prime.FirstPrimeResponse> responseObserver) {
      io.grpc.stub.ClientCalls.asyncUnaryCall(
          getChannel().newCall(getFirstPrimeMethod(), getCallOptions()), request, responseObserver);
    }

    /**
     */
    public void intervalPrime(prime.Prime.IntervalRequest request,
        io.grpc.stub.StreamObserver<prime.Prime.PrimeNumber> responseObserver) {
      io.grpc.stub.ClientCalls.asyncServerStreamingCall(
          getChannel().newCall(getIntervalPrimeMethod(), getCallOptions()), request, responseObserver);
    }
  }

  /**
   * A stub to allow clients to do synchronous rpc calls to service PrimeService.
   */
  public static final class PrimeServiceBlockingStub
      extends io.grpc.stub.AbstractBlockingStub<PrimeServiceBlockingStub> {
    private PrimeServiceBlockingStub(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected PrimeServiceBlockingStub build(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      return new PrimeServiceBlockingStub(channel, callOptions);
    }

    /**
     */
    public prime.Prime.PrimeResponse isPrime(prime.Prime.PrimeRequest request) {
      return io.grpc.stub.ClientCalls.blockingUnaryCall(
          getChannel(), getIsPrimeMethod(), getCallOptions(), request);
    }

    /**
     */
    public prime.Prime.FirstPrimeResponse firstPrime(prime.Prime.FirstPrimeRequest request) {
      return io.grpc.stub.ClientCalls.blockingUnaryCall(
          getChannel(), getFirstPrimeMethod(), getCallOptions(), request);
    }

    /**
     */
    public java.util.Iterator<prime.Prime.PrimeNumber> intervalPrime(
        prime.Prime.IntervalRequest request) {
      return io.grpc.stub.ClientCalls.blockingServerStreamingCall(
          getChannel(), getIntervalPrimeMethod(), getCallOptions(), request);
    }
  }

  /**
   * A stub to allow clients to do ListenableFuture-style rpc calls to service PrimeService.
   */
  public static final class PrimeServiceFutureStub
      extends io.grpc.stub.AbstractFutureStub<PrimeServiceFutureStub> {
    private PrimeServiceFutureStub(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected PrimeServiceFutureStub build(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      return new PrimeServiceFutureStub(channel, callOptions);
    }

    /**
     */
    public com.google.common.util.concurrent.ListenableFuture<prime.Prime.PrimeResponse> isPrime(
        prime.Prime.PrimeRequest request) {
      return io.grpc.stub.ClientCalls.futureUnaryCall(
          getChannel().newCall(getIsPrimeMethod(), getCallOptions()), request);
    }

    /**
     */
    public com.google.common.util.concurrent.ListenableFuture<prime.Prime.FirstPrimeResponse> firstPrime(
        prime.Prime.FirstPrimeRequest request) {
      return io.grpc.stub.ClientCalls.futureUnaryCall(
          getChannel().newCall(getFirstPrimeMethod(), getCallOptions()), request);
    }
  }

  private static final int METHODID_IS_PRIME = 0;
  private static final int METHODID_FIRST_PRIME = 1;
  private static final int METHODID_INTERVAL_PRIME = 2;

  private static final class MethodHandlers<Req, Resp> implements
      io.grpc.stub.ServerCalls.UnaryMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.ServerStreamingMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.ClientStreamingMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.BidiStreamingMethod<Req, Resp> {
    private final AsyncService serviceImpl;
    private final int methodId;

    MethodHandlers(AsyncService serviceImpl, int methodId) {
      this.serviceImpl = serviceImpl;
      this.methodId = methodId;
    }

    @java.lang.Override
    @java.lang.SuppressWarnings("unchecked")
    public void invoke(Req request, io.grpc.stub.StreamObserver<Resp> responseObserver) {
      switch (methodId) {
        case METHODID_IS_PRIME:
          serviceImpl.isPrime((prime.Prime.PrimeRequest) request,
              (io.grpc.stub.StreamObserver<prime.Prime.PrimeResponse>) responseObserver);
          break;
        case METHODID_FIRST_PRIME:
          serviceImpl.firstPrime((prime.Prime.FirstPrimeRequest) request,
              (io.grpc.stub.StreamObserver<prime.Prime.FirstPrimeResponse>) responseObserver);
          break;
        case METHODID_INTERVAL_PRIME:
          serviceImpl.intervalPrime((prime.Prime.IntervalRequest) request,
              (io.grpc.stub.StreamObserver<prime.Prime.PrimeNumber>) responseObserver);
          break;
        default:
          throw new AssertionError();
      }
    }

    @java.lang.Override
    @java.lang.SuppressWarnings("unchecked")
    public io.grpc.stub.StreamObserver<Req> invoke(
        io.grpc.stub.StreamObserver<Resp> responseObserver) {
      switch (methodId) {
        default:
          throw new AssertionError();
      }
    }
  }

  public static final io.grpc.ServerServiceDefinition bindService(AsyncService service) {
    return io.grpc.ServerServiceDefinition.builder(getServiceDescriptor())
        .addMethod(
          getIsPrimeMethod(),
          io.grpc.stub.ServerCalls.asyncUnaryCall(
            new MethodHandlers<
              prime.Prime.PrimeRequest,
              prime.Prime.PrimeResponse>(
                service, METHODID_IS_PRIME)))
        .addMethod(
          getFirstPrimeMethod(),
          io.grpc.stub.ServerCalls.asyncUnaryCall(
            new MethodHandlers<
              prime.Prime.FirstPrimeRequest,
              prime.Prime.FirstPrimeResponse>(
                service, METHODID_FIRST_PRIME)))
        .addMethod(
          getIntervalPrimeMethod(),
          io.grpc.stub.ServerCalls.asyncServerStreamingCall(
            new MethodHandlers<
              prime.Prime.IntervalRequest,
              prime.Prime.PrimeNumber>(
                service, METHODID_INTERVAL_PRIME)))
        .build();
  }

  private static abstract class PrimeServiceBaseDescriptorSupplier
      implements io.grpc.protobuf.ProtoFileDescriptorSupplier, io.grpc.protobuf.ProtoServiceDescriptorSupplier {
    PrimeServiceBaseDescriptorSupplier() {}

    @java.lang.Override
    public com.google.protobuf.Descriptors.FileDescriptor getFileDescriptor() {
      return prime.Prime.getDescriptor();
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.ServiceDescriptor getServiceDescriptor() {
      return getFileDescriptor().findServiceByName("PrimeService");
    }
  }

  private static final class PrimeServiceFileDescriptorSupplier
      extends PrimeServiceBaseDescriptorSupplier {
    PrimeServiceFileDescriptorSupplier() {}
  }

  private static final class PrimeServiceMethodDescriptorSupplier
      extends PrimeServiceBaseDescriptorSupplier
      implements io.grpc.protobuf.ProtoMethodDescriptorSupplier {
    private final java.lang.String methodName;

    PrimeServiceMethodDescriptorSupplier(java.lang.String methodName) {
      this.methodName = methodName;
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.MethodDescriptor getMethodDescriptor() {
      return getServiceDescriptor().findMethodByName(methodName);
    }
  }

  private static volatile io.grpc.ServiceDescriptor serviceDescriptor;

  public static io.grpc.ServiceDescriptor getServiceDescriptor() {
    io.grpc.ServiceDescriptor result = serviceDescriptor;
    if (result == null) {
      synchronized (PrimeServiceGrpc.class) {
        result = serviceDescriptor;
        if (result == null) {
          serviceDescriptor = result = io.grpc.ServiceDescriptor.newBuilder(SERVICE_NAME)
              .setSchemaDescriptor(new PrimeServiceFileDescriptorSupplier())
              .addMethod(getIsPrimeMethod())
              .addMethod(getFirstPrimeMethod())
              .addMethod(getIntervalPrimeMethod())
              .build();
        }
      }
    }
    return result;
  }
}
