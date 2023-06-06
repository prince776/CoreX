#pragma once

#include <CoreX/concepts.hpp>
#include <CoreX/utility.hpp>
#include <CoreX/variant.hpp>
#include <cassert>
#include <string>

template <typename T, typename E>
class Result;

// ResultValueProcessor is concept for functions that can process Result<T1,E1>.
// It should take parameter T1& and return Result<T2, E2>.
// Since this processor function should be able to be called for input results
// that have error and not value, E2 should be constructible from E1.
template <typename Fn, typename T1, typename E1, typename T2, typename E2>
concept ResultValueProcessor = requires(Fn fn, T1& t1, E1 e1, E2 e2) {
    { fn(t1) } -> same_as<Result<T2, E2>>;
    { E2(e1) } -> same_as<E2>;
};

// ResultErrorProcessor is concept for functions that can process error from
// Result<T1,E1>. It should take parameter E1& and return Result<T2, E2>.
// When there's no error, then empty Result<T2, E2> will be returned, so
// T2 should be default constructible.
template <typename Fn, typename T1, typename E1, typename T2, typename E2>
concept ResultErrorProcessor = requires(Fn fn, E1& e1, E2 e2) {
    { fn(e1) } -> same_as<Result<T2, E2>>;
    { T2{} };
};

template <typename T, typename E>
class Result {
  private:
    Result() = default;

  public:
    Result(const T& t) : data(t) {
    }

    static Result FromErr(const E& e) {
        Result res;
        res.data = e;
        return res;
    }

    [[nodiscard]] bool hasError() const noexcept {
        return data.template holdsAlternative<E>();
    }

    [[nodiscard]] E& error() noexcept {
        assert(hasError());
        return data.template get<E>();
    }

    [[nodiscard]] T& value() noexcept {
        assert(!hasError());
        return data.template get<T>();
    }

    // then takes a ResultValueProcessor function, that should take in this
    // result's value, and return another Result.
    // If there was error in this Result then the error is simply propagated
    // to the output without calling the provided fn.
    // For processing error, see onError.
    // Since we pass the error to Result<T2,E2>, the returned result's error
    // should be construcitble from this object's error.
    template <typename T2, typename E2, ResultValueProcessor<T, E, T2, E2> Fn>
    [[nodiscard]] Result<T2, E2> then(Fn fn) noexcept {
        if (hasError()) {
            return Result<T2, E2>::FromErr(E2{error()});
        }
        return fn(value());
    }

    // onError takes a ResultErrorProcessor function, that should take in this
    // result's error, and return another Result.
    // If there was no error in this Result then final Result is constructed by
    // calling default constructor of T2, hence it should be default
    // constructible.
    // For processing value, see then.
    template <typename T2, typename E2, ResultErrorProcessor<T, E, T2, E2> Fn>
    [[nodiscard]] Result<T2, E2> onError(Fn fn) noexcept {
        if (!hasError()) {
            return Result<T2, E2>(T2{});
        }
        return fn(error());
    }

  private:
    Variant<T, E> data;
};

template <typename T>
class Result<T, T> {
  private:
    Result() = default;

  public:
    Result(const T& t) : data(t), hasErr(false) {
    }

    static Result FromErr(const T& e) {
        Result res;
        res.data   = e;
        res.hasErr = true;
        return res;
    }

    [[nodiscard]] bool hasError() const noexcept {
        return hasErr;
    }

    [[nodiscard]] T& error() noexcept {
        assert(hasError());
        return data;
    }

    [[nodiscard]] T& value() noexcept {
        assert(!hasError());
        return data;
    }

    // then takes a ResultValueProcessor function, that should take in this
    // result's value, and return another Result.
    // If there was error in this Result then the error is simply propagated
    // to the output without calling the provided fn.
    // For processing error, see onError.
    // Since we pass the error to Result<T2,E2>, the returned result's error
    // should be construcitble from this object's error.
    template <typename T2, typename E2, ResultValueProcessor<T, T, T2, E2> Fn>
    [[nodiscard]] Result<T2, E2> then(Fn fn) noexcept {
        if (hasError()) {
            return Result<T2, E2>::FromErr(E2{error()});
        }
        return fn(value());
    }

    // onError takes a ResultErrorProcessor function, that should take in this
    // result's error, and return another Result.
    // If there was no error in this Result then final Result is constructed by
    // calling default constructor of T2, hence it should be default
    // constructible.
    // For processing value, see then.
    template <typename T2, typename E2, ResultErrorProcessor<T, T, T2, E2> Fn>
    [[nodiscard]] Result<T2, E2> onError(Fn fn) noexcept {
        if (!hasError()) {
            return Result<T2, E2>(T2{});
        }
        return fn(error());
    }

  private:
    T data;
    bool hasErr = false;
};
