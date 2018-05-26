# Decoding API Response JSON in ReasonReact

Reason is a new statically-typed functional programming language from Facebook which can be compiled to Javascript. Reason React is a wrapper for React which makes it easy to use from Reason.

We're going to build a small single page web app to put Reason React through its paces. The app will display a list of top Reason-related Github repos. It's a small enough task that we can complete it in a few hours, but also has enough complexity that we can kick the tires of this new language. This tutorial expects no existing knowledge of Reason, though a basic familiarity with static types would be helpful.

# A New roject

We're going to use `create-react-app` using `reason-scripts` which will create a starting point for our app, which is going to be called `decoding-reason`:

```
yarn create react-app decoding-reason -- --scripts-version reason-scripts
```

Start the project by running `yarn start` which should open `https://localhost:3000` in your browser.

# A Record Type

We will be decoding a response from the public data from this Github API request: https://api.github.com/search/repositories?q=topic%3Areasonml&type=Repositories but first lets set it up using fake data.

Create a new file called RepoData.re and add this code into it.

```
type repo = {
  full_name: string,
  stargazers_count: int,
  html_url: string
};
```
