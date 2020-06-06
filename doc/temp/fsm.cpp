// www.youtube.com/watch?v=gKbORJtnVu8

struct event_connect
{
    std::string_view address;
};
struct event_connected
{};
struct event_disconnect
{};
struct event_timeout
{};
// using event = std::variant<event_connect, event_connected, event_disconnect, event_timeout>;

struct state_idle
{};
struct state_connecting
{
    static constexpr int n_max = 3;
    int                  n     = 0;
    std::string          address;
};
struct state_connected
{};
using state = std::variant<state_idle, state_connecting, state_connected>;

template<typename Derived, typename StateVariant>
class fsm
{
    StateVariant state_;

public:
    template<typename Event>
    void dispatch(Event && event)
    {
        Derived & child     = static_cast<Derived &>(*this);
        auto      new_state = std::visit(
            [&](auto & s) -> std::optional<StateVariant> {
                return child.on_event(s, std::forward<Event>(event));
            },
            state_);
        if(new_state)
            state_ = *std::move(new_state);
    }
};

class connection_fsm : public fsm<connection_fsm, state>
{
public:
    auto on_event(state_idle &, const event_connect & e) { return state_connecting{std::string(e.address)}; }
    auto on_event(state_connecting &, const event_connected &) { return state_connected{}; }
    auto on_event(state_connecting & s, const event_timeout &)
    {
        return ++s.n < state_connecting::n_max ? std::nullopt : std::optional<state>(state_idle{});
    }
    auto on_event(state_connected &, const event_disconnect &) { return state_idle{}; }
    template<typename State, typename Event>
    auto on_event(State &, const Event &)
    {
        return std::nullopt;
    }
};
