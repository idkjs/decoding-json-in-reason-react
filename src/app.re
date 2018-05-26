[%bs.raw {|require('./app.css')|}];

[@bs.module] external logo : string = "./logo.svg";

let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self =>
    <div className="App">
      <h1> (ReasonReact.string("Decoding Graphql in Reason")) </h1>
    </div>,
};